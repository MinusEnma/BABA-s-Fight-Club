#ifndef CARD_H
#define CARD_H
 
#include <stdint.h>
#include "../fightingStyle/fightingStyle.h"
 
#define CARD_NAME_LEN 32
#define CARD_DESC_LEN 96
 
typedef enum {
    CARD_ATTACK,
    CARD_BLOCK,
    CARD_EVADE
} CardType;
 
typedef struct {
    char name[CARD_NAME_LEN];
    char description[CARD_DESC_LEN];
 
    uint8_t type;       // CardType
    uint8_t style;      // FightingStyle
    uint8_t energy_cost;
    uint8_t damage;          // base damage, 0 for block/evade
    uint8_t defense_value;   // base block amount or base evade value
    uint8_t stagger_damage;
    uint8_t dice_min;
    uint8_t dice_max;
} Card;
 
int card_roll_dice(const Card *c);
int card_calculate_damage(const Card *c, int roll);    // attack: base damage + roll
int card_calculate_defense(const Card *c, int roll);   // block/evade: base defense value + roll
 
#endif