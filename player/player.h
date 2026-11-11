#ifndef PLAYER_H
#define PLAYER_H
 
#include <stdint.h>
#include <stdbool.h>
#include "../card/card.h"
#include "../card/cardDatabase.h"

#define PLAYER_NAME_LEN 32
#define MAX_DECK   64      // max cards in the deck / draw pile / discard pile
#define MAX_HAND   10
#define MAX_COMBO  10
#define HAND_SIZE  6       // cards drawn per round
 
#define START_HP       250
#define START_STAGGER  125
#define ROUND_ENERGY   5   // placeholder: energy per round
 
// Evade tuning (placeholders)
#define EVADE_DECAY      2     // evade value lost after each successful evade (makes failure more likely)
#define EVADE_TIES_HOLD  1     // 1: attack roll == evade still evades (attack must be HIGHER to beat it)
                               // 0: attack must be strictly LOWER to be evaded
#define EVADE_MAX  11    // an attack d12 can roll 12, so evade 12+ would be unbeatable

// Piles store card IDs (see CardId in cardDatabase.h), not whole cards.
typedef struct {
    char       name[PLAYER_NAME_LEN];
    uint8_t    style;             // FightingStyle
    StyleStats stats;

    int16_t hp, max_hp, stagger, energy, defense, speed;
    uint8_t has_initiative, stunned;


    int16_t block;                // Slay-the-Spire style pool: absorbs damage before HP, expires after the turn
    int16_t evade;                // 0 = none active, else the threshold an attack roll is compared to; lasts for the clash

    uint8_t deck[MAX_DECK];       uint8_t deck_count;      // draw pile
    uint8_t hand[MAX_HAND];       uint8_t hand_count;
    uint8_t discard[MAX_DECK];    uint8_t discard_count;
    uint8_t combo[MAX_COMBO];     uint8_t combo_count;     // queued moves
} Player;

 
// Result of one attack landing on a player (handy for terminal output)
typedef struct {
    int  blocked;         // damage absorbed by block
    int  hp_lost;         // damage that reached HP
    bool evaded;          // attack was dodged (evade holds)
    bool evade_broken;    // evade was beaten by this attack
} HitResult;

// Setup
void player_init(Player *p);                                            // default player
void player_create(Player *p, const char *name, FightingStyle style);   // named player with style stats
void player_build_deck(Player *p);                                      // loads the starter deck
void player_shuffle_deck(Player *p);

// Deck handling
bool player_draw_card(Player *p);       // draws 1 (reshuffles discard if deck is empty)
void player_draw_hand(Player *p);       // draws until the hand has HAND_SIZE cards
void player_discard_hand(Player *p);

// Turn / combo
int  player_roll_initiative(const Player *p);   // d20 + speed
bool player_queue_combo(Player *p, int hand_index);
void player_clear_queue(Player *p);

// Combat
int  player_roll_card(const Player *p, const Card *c);   // rolls the card's own dice
int  player_roll_defense(const Player *p);      // d6 + defense + defense roll bonus
void player_take_damage(Player *p, int amount);
void player_take_stagger_damage(Player *p, int amount);
bool player_is_stunned(const Player *p);
 
// Defense
void      player_activate_defense(Player *p, const Card *c);                  // roll a BLOCK/EVADE card into block/evade
HitResult player_receive_attack(Player *p, const Card *atk, int attack_roll); // block -> evade -> HP
void      player_end_turn(Player *p);                                         // block expires (evade stays until round end)
 
// Round handling.
// Start of game:  player_build_deck();  player_reset_round();
// After a clash:  player_reset_round();
void player_reset_round(Player *p);

 
#endif
