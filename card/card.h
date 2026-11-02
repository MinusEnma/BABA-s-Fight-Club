#ifndef CARD_H
#define CARD_H

#include <stdint.h>

#define CARD_NAME_LEN 32
#define CARD_DESC_LEN 96

typedef enum {
    STYLE_BOXING,
    STYLE_MUAY_THAI,
    STYLE_TAEKWONDO,
    STYLE_BJJ,
    STYLE_KARATE,
    STYLE_COUNT
} FightingStyle;

typedef enum {
    CARD_ATTACK,
    CARD_BLOCK,
    CARD_EVADE
} CardType;

// Fixed size: no pointers, no dynamic memory.
// Enums are stored as uint8_t so the layout stays small and predictable.
typedef struct {
    char    name[CARD_NAME_LEN];
    char    description[CARD_DESC_LEN];
    uint8_t type;            // CardType
    uint8_t style;           // FightingStyle
    uint8_t energy_cost;
    uint8_t damage;          // base damage, 0 for block/evade
    uint8_t stagger_damage;
    uint8_t dice_min;
    uint8_t dice_max;
} Card;

int card_roll_dice(const Card *c);                    // roll between dice_min and dice_max
int card_calculate_damage(const Card *c, int roll);   // base damage + roll

#endif