#include <stdio.h>
#include <string.h>
#include "player.h"
#include "dice.h"

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
    p->energy = 0;
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
int player_roll_attack(const Player *p) {
    (void)p;
    return roll_attack();
}

int player_roll_defense(const Player *p) {
    return roll(6) + p->stats.defense + p->stats.defense_roll_bonus;
}

void player_take_damage(Player *p, int amount) {
    p->hp -= amount;
    if (p->hp < 0) p->hp = 0;
}

void player_take_stagger_damage(Player *p, int amount) {
    p->stagger -= amount;
    if (p->stagger <= 0) {
        p->stagger = 0;
        p->stunned = 1;
        player_clear_queue(p);   // played cards are nullified and go to the discard pile
    }
}

bool player_is_stunned(const Player *p) {
    return p->stunned != 0;
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
    p->energy = ROUND_ENERGY;
    p->has_initiative = 0;
}