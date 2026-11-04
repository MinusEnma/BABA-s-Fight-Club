#ifndef FIGHTING_STYLE_H
#define FIGHTING_STYLE_H

#include <stdint.h>

typedef enum {
    STYLE_BOXING,
    STYLE_MUAY_THAI,
    STYLE_TAEKWONDO,
    STYLE_BJJ,
    STYLE_KARATE,
    STYLE_COUNT
} FightingStyle;

typedef struct {
    uint8_t defense;
    uint8_t speed;
    uint8_t defense_roll_bonus;
    uint8_t evade_bonus;
} StyleStats;

StyleStats get_style_stats(FightingStyle style);

#endif