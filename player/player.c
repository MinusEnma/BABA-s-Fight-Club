#include <stdio.h>
#include <string.h>
#include "player.h"
#include "../dice/dice.h"
#include "../fightingStyle/fightingStyle.h"

// ---------------------------------------------------------------
// Setup
// ---------------------------------------------------------------
void player_init(Player *p) {
    memset(p, 0, sizeof(*p));
    snprintf(p->name, PLAYER_NAME_LEN, "%s", "Unknown");
    p->style = STYLE_BOXING;                 // placeholder default
    p->stats = (StyleStats){5, 5, 0, 0};

    p->hp = p->max_hp = START_HP;
    p->stagger = START_STAGGER;
    p->energy = 0;
    p->defense = p->stats.defense;
    p->speed = p->stats.speed;
}

void player_create(Player *p, const char *name, FightingStyle style) {
    memset(p, 0, sizeof(*p));
    snprintf(p->name, PLAYER_NAME_LEN, "%s", name);
    p->style = (uint8_t)style;
    p->stats = get_style_stats(style);

    p->hp = p->max_hp = START_HP;            // same for every style
    p->stagger = START_STAGGER;              // same for every style
    p->energy = ROUND_ENERGY;
    p->defense = p->stats.defense;
    p->speed = p->stats.speed;
}

void player_build_deck(Player *p) {
    p->hand_count = 0;
    p->discard_count = 0;
    p->combo_count = 0;

    p->deck_count = (uint8_t)get_starter_deck((FightingStyle)p->style, p->deck, MAX_DECK);
    player_shuffle_deck(p);
}

void player_shuffle_deck(Player *p) {
    // Fisher-Yates shuffle
    for (int i = p->deck_count - 1; i > 0; i--) {
        int j = roll_range(0, i);
        uint8_t tmp = p->deck[i];
        p->deck[i] = p->deck[j];
        p->deck[j] = tmp;
    }
}


// Deck handling
bool player_draw_card(Player *p) {
    if (p->hand_count >= MAX_HAND) return false;

    // Draw pile empty? Shuffle the discard pile back into it
    if (p->deck_count == 0) {
        if (p->discard_count == 0) return false;   // nothing left anywhere
        memcpy(p->deck, p->discard, p->discard_count);
        p->deck_count = p->discard_count;
        p->discard_count = 0;
        player_shuffle_deck(p);
    }

    p->deck_count--;
    p->hand[p->hand_count++] = p->deck[p->deck_count];
    return true;
}

void player_draw_hand(Player *p) {
    while (p->hand_count < HAND_SIZE) {
        if (!player_draw_card(p)) break;
    }
}

void player_discard_hand(Player *p) {
    for (int i = 0; i < p->hand_count && p->discard_count < MAX_DECK; i++) {
        p->discard[p->discard_count++] = p->hand[i];
    }
    p->hand_count = 0;
}

// Turn / combos

int player_roll_initiative(const Player *p) {
    return roll_initiative() + p->speed;
}

bool player_queue_combo(Player *p, int hand_index) {
    if (hand_index < 0 || hand_index >= p->hand_count) return false;
    if (p->combo_count >= MAX_COMBO) return false;

    const Card *c = card_get(p->hand[hand_index]);
    if (c == NULL) return false;
    if (c->energy_cost > p->energy) return false;      // not enough energy

    p->energy -= c->energy_cost;
    p->combo[p->combo_count++] = p->hand[hand_index];

    // remove the card from the hand (shift the rest down)
    for (int i = hand_index; i < p->hand_count - 1; i++) {
        p->hand[i] = p->hand[i + 1];
    }
    p->hand_count--;
    return true;
}

void player_clear_queue(Player *p) {
    // Played cards go to the discard pile
    for (int i = 0; i < p->combo_count && p->discard_count < MAX_DECK; i++) {
        p->discard[p->discard_count++] = p->combo[i];
    }
    p->combo_count = 0;
}

// ---------------------------------------------------------------
// Combat
// ---------------------------------------------------------------
int player_roll_attack(const Player *p, const Card *c) {
    (void)p;
    return card_roll_dice(c);      // uses the card's dice_min..dice_max
}

int player_roll_defense(const Player *p) {
    return roll(6) + p->defense + p->stats.defense_roll_bonus;
}

void player_take_damage(Player *p, int amount) {
    p->hp -= amount;
    if (p->hp < 0) p->hp = 0;
}

void player_take_stagger_damage(Player *p, int amount) {
    if (p->stunned) return;          // already stunned: don't re-trigger
    p->stagger -= amount;
    if (p->stagger <= 0) {
        p->stagger = 0;
        p->stunned = 1;
        player_clear_queue(p);
        p->block = 0;
        p->evade = 0;
    }
}

bool player_is_stunned(const Player *p) {
    return p->stunned != 0;
}

// ---------------------------------------------------------------
// Defense (block / evade)
// ---------------------------------------------------------------
// Called when a BLOCK or EVADE card is played.
void player_activate_defense(Player *p, const Card *c) {
    if (p == NULL || c == NULL) return;

    int value = card_roll_dice(c);

    if (c->type == CARD_BLOCK) {
        // defense stat now actually matters
        p->block += value + p->defense + p->stats.defense_roll_bonus;
    } else if (c->type == CARD_EVADE) {
        value += p->stats.evade_bonus;
        if (value > EVADE_MAX) value = EVADE_MAX;          // keep evade beatable
        if (value > p->evade) p->evade = (int16_t)value;   // keep the stronger evade
    }
}

// Resolves one attack against this player.
// attack_roll is the raw dice roll, which is what gets tested against evade.
HitResult player_receive_attack(Player *p, const Card *atk, int attack_roll) {
    HitResult r = {0};
    if (p == NULL || atk == NULL) return r;
    int dmg = card_calculate_damage(atk, attack_roll);

    // 1) Block soaks first (like Slay the Spire). Evade is not touched by absorbed hits.
    if (p->block > 0) {
        int absorbed = (dmg < p->block) ? dmg : p->block;
        p->block -= (int16_t)absorbed;
        dmg -= absorbed;
        r.blocked = absorbed;
    }
    if (dmg <= 0) return r;

    // 2) Damage that got past block faces the evade.
    if (p->evade > 0) {
        bool beaten = EVADE_TIES_HOLD ? (attack_roll > p->evade)
                                      : (attack_roll >= p->evade);
        if (beaten) {
            p->evade = 0;              // beaten: evade is gone and the hit lands
            r.evade_broken = true;
        } else {
            // Evade holds, but gets weaker: failure becomes more likely next time
            p->evade -= EVADE_DECAY;
            if (p->evade < 0) p->evade = 0;
            r.evaded = true;
            return r;
        }
    }

    // 3) Damage reaches HP
    player_take_damage(p, dmg);
    player_take_stagger_damage(p, atk->stagger_damage);
    r.hp_lost = dmg;
    return r;
}

// Call when a player's turn ends: block expires, evade is retained.
void player_end_turn(Player *p) {
    p->block = 0;
}

// ---------------------------------------------------------------
// Round handling
// ---------------------------------------------------------------
void player_reset_round(Player *p) {
    player_clear_queue(p);      // played cards -> discard
    player_discard_hand(p);     // unplayed cards -> discard
    player_draw_hand(p);        // draw a fresh hand

    p->stagger = START_STAGGER;
    p->stunned = 0;
    p->block = 0;               // the clash is over, so both defenses end
    p->evade = 0;
    p->energy = ROUND_ENERGY;
    p->has_initiative = 0;
}
