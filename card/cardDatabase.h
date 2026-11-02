#ifndef CARDDATABASE_H
#define CARDDATABASE_H

#include <stdint.h>
#include "card.h"

// Every card has a fixed ID (its index in the card table).
// Players store these small IDs in their piles, so only IDs
// need to travel over the socket, never names or descriptions.
typedef enum {
    CARD_BOX_JAB,
    CARD_BOX_CROSS,
    CARD_BOX_LEAD_HOOK,
    CARD_BOX_UPPERCUT,
    CARD_BOX_BODY_SHOT,
    CARD_BOX_JAB_CROSS,
    CARD_BOX_HOOK_CROSS,
    CARD_BOX_COUNTER_PUNCH,
    CARD_BOX_SLIP_DODGE,
    CARD_BOX_BOXING_GUARD,
    CARD_BOX_PARRY,
    CARD_MT_TEEP,
    CARD_MT_ROUNDHOUSE_KICK,
    CARD_MT_LOW_KICK,
    CARD_MT_BODY_KICK,
    CARD_MT_KNEE_STRIKE,
    CARD_MT_ELBOW_STRIKE,
    CARD_MT_CLINCH_KNEE_COMBO,
    CARD_MT_SPINNING_ELBOW,
    CARD_MT_KICK_KNEE_COMBO,
    CARD_MT_SWAY_DODGE,
    CARD_MT_THAI_GUARD,
    CARD_MT_LEG_CHECK,
    CARD_TKD_FRONT_KICK,
    CARD_TKD_SIDE_KICK,
    CARD_TKD_ROUNDHOUSE_KICK,
    CARD_TKD_AXE_KICK,
    CARD_TKD_BACK_KICK,
    CARD_TKD_SPINNING_HOOK_KICK,
    CARD_TKD_JUMPING_FRONT_KICK,
    CARD_TKD_DOUBLE_ROUNDHOUSE,
    CARD_TKD_TORNADO_KICK,
    CARD_TKD_BACKSTEP_DODGE,
    CARD_TKD_HIGH_GUARD,
    CARD_TKD_SIDE_STEP_GUARD,
    CARD_BJJ_SINGLE_LEG_TAKEDOWN,
    CARD_BJJ_DOUBLE_LEG_TAKEDOWN,
    CARD_BJJ_GUARD_SWEEP,
    CARD_BJJ_ARMBAR,
    CARD_BJJ_TRIANGLE_CHOKE,
    CARD_BJJ_REAR_NAKED_CHOKE,
    CARD_BJJ_KIMURA_LOCK,
    CARD_BJJ_MOUNT_CONTROL,
    CARD_BJJ_GROUND_AND_POUND,
    CARD_BJJ_SWEEP_ARMBAR_COMBO,
    CARD_BJJ_GUARD_RETENTION_DODGE,
    CARD_BJJ_CLOSED_GUARD_BLOCK,
    CARD_BJJ_SUBMISSION_ESCAPE,
    CARD_BJJ_SPRAWL,
    CARD_KAR_STRAIGHT_PUNCH,
    CARD_KAR_REVERSE_PUNCH,
    CARD_KAR_KNIFE_HAND_STRIKE,
    CARD_KAR_FRONT_KICK,
    CARD_KAR_SIDE_KICK,
    CARD_KAR_ROUNDHOUSE_KICK,
    CARD_KAR_BACKFIST,
    CARD_KAR_SPINNING_BACK_KICK,
    CARD_KAR_KARATE_COMBINATION,
    CARD_KAR_COUNTER_STRIKE,
    CARD_KAR_TAI_SABAKI_DODGE,
    CARD_KAR_KARATE_GUARD,
    CARD_KAR_COUNTER_BLOCK,
    CARD_COUNT
} CardId;

// Stats that differ per fighting style (HP and stagger are the same for all)
typedef struct {
    uint8_t defense;
    uint8_t speed;
    uint8_t defense_roll_bonus;   // added to defense rolls
    uint8_t evade_bonus;          // added to evade rolls
} StyleStats;

StyleStats get_style_stats(FightingStyle style);

// Returns the card with this ID, or NULL if the ID is invalid
const Card *card_get(uint8_t id);

// Fills out[] with the IDs of every card belonging to a style.
// Returns how many were written (at most max).
int card_pool_for_style(FightingStyle style, uint8_t *out, int max);

// Fills out[] with the predetermined starter deck IDs for a style.
// Returns how many were written (at most max).
int get_starter_deck(FightingStyle style, uint8_t *out, int max);

#endif