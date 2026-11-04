#include <stddef.h>
#include "cardDatabase.h"
#include "../dice/dice.h"

// name, description, type, style, energy_cost, damage,
// stagger_damage, dice_min, dice_max
#define CARD_ROW(n, d, t, s, cost, dmg, stg, dmin, dmax) \
    { n, d, (uint8_t)(t), (uint8_t)(s), cost, dmg, stg, dmin, dmax }

static const Card CARD_TABLE[CARD_COUNT] = {

    // ---------- BOXING ----------
    [CARD_BOX_JAB] = CARD_ROW("Jab", "A quick punch to the opponent", CARD_ATTACK, STYLE_BOXING, 1, 10, 5, 1, 12),
    [CARD_BOX_CROSS] = CARD_ROW("Cross", "A powerful, straight punch delivered with the rear hand.", CARD_ATTACK, STYLE_BOXING, 2, 18, 5, 1, 12),
    [CARD_BOX_LEAD_HOOK] = CARD_ROW("Lead Hook", "A curved punch targeting the side of the opponent.", CARD_ATTACK, STYLE_BOXING, 3, 25, 5, 1, 12),
    [CARD_BOX_UPPERCUT] = CARD_ROW("Uppercut", "An upward punch aimed at the opponent's chin.", CARD_ATTACK, STYLE_BOXING, 3, 25, 5, 1, 12),
    [CARD_BOX_BODY_SHOT] = CARD_ROW("Body Shot", "A punch aimed at the opponent's torso.", CARD_ATTACK, STYLE_BOXING, 2, 16, 5, 1, 12),
    [CARD_BOX_JAB_CROSS] = CARD_ROW("Jab-Cross", "Combines a quick jab with a powerful cross.", CARD_ATTACK, STYLE_BOXING, 3, 28, 5, 1, 12),
    [CARD_BOX_HOOK_CROSS] = CARD_ROW("Hook-Cross", "Delivers a hook followed by a straight cross.", CARD_ATTACK, STYLE_BOXING, 4, 35, 5, 1, 12),
    [CARD_BOX_COUNTER_PUNCH] = CARD_ROW("Counter Punch", "Strikes immediately after the opponent misses.", CARD_ATTACK, STYLE_BOXING, 2, 18, 5, 1, 12),
    [CARD_BOX_SLIP_DODGE] = CARD_ROW("Slip Dodge", "Quickly moves the head aside to evade a punch.", CARD_EVADE, STYLE_BOXING, 1, 0, 0, 1, 12),
    [CARD_BOX_BOXING_GUARD] = CARD_ROW("Boxing Guard", "Raises the arms to absorb incoming damage.", CARD_BLOCK, STYLE_BOXING, 1, 0, 0, 1, 12),
    [CARD_BOX_PARRY] = CARD_ROW("Parry", "Deflects an incoming punch with precise timing.", CARD_BLOCK, STYLE_BOXING, 2, 0, 0, 1, 12),

    // ---------- MUAY THAI ----------
    [CARD_MT_TEEP] = CARD_ROW("Teep", "Pushes the opponent back with a straight front kick.", CARD_ATTACK, STYLE_MUAY_THAI, 1, 12, 5, 1, 12),
    [CARD_MT_ROUNDHOUSE_KICK] = CARD_ROW("Roundhouse Kick", "Swings the leg in a powerful circular kick.", CARD_ATTACK, STYLE_MUAY_THAI, 3, 24, 5, 1, 12),
    [CARD_MT_LOW_KICK] = CARD_ROW("Low Kick", "Targets the opponent's thigh with a low kick.", CARD_ATTACK, STYLE_MUAY_THAI, 2, 16, 5, 1, 12),
    [CARD_MT_BODY_KICK] = CARD_ROW("Body Kick", "Delivers a powerful kick to the torso.", CARD_ATTACK, STYLE_MUAY_THAI, 2, 22, 5, 1, 12),
    [CARD_MT_KNEE_STRIKE] = CARD_ROW("Knee Strike", "Drives the knee forward into the opponent.", CARD_ATTACK, STYLE_MUAY_THAI, 2, 23, 5, 1, 12),
    [CARD_MT_ELBOW_STRIKE] = CARD_ROW("Elbow Strike", "Delivers a sharp, close-range elbow strike.", CARD_ATTACK, STYLE_MUAY_THAI, 3, 26, 5, 1, 12),
    [CARD_MT_CLINCH_KNEE_COMBO] = CARD_ROW("Clinch Knee Combo", "Controls the opponent before landing two knee strikes.", CARD_ATTACK, STYLE_MUAY_THAI, 4, 32, 5, 1, 12),
    [CARD_MT_SPINNING_ELBOW] = CARD_ROW("Spinning Elbow", "Rotates quickly to deliver a powerful elbow strike.", CARD_ATTACK, STYLE_MUAY_THAI, 4, 34, 5, 1, 12),
    [CARD_MT_KICK_KNEE_COMBO] = CARD_ROW("Kick-Knee Combo", "Follows a kick with a driving knee strike.", CARD_ATTACK, STYLE_MUAY_THAI, 4, 35, 5, 1, 12),
    [CARD_MT_SWAY_DODGE] = CARD_ROW("Sway Dodge", "Leans away to evade an incoming close-range strike.", CARD_EVADE, STYLE_MUAY_THAI, 1, 0, 0, 1, 12),
    [CARD_MT_THAI_GUARD] = CARD_ROW("Thai Guard", "Uses a tight guard to absorb incoming strikes.", CARD_BLOCK, STYLE_MUAY_THAI, 2, 0, 0, 1, 12),
    [CARD_MT_LEG_CHECK] = CARD_ROW("Leg Check", "Raises the leg to defend against an incoming kick.", CARD_BLOCK, STYLE_MUAY_THAI, 1, 0, 0, 1, 12),

    // ---------- TAEKWONDO ----------
    [CARD_TKD_FRONT_KICK] = CARD_ROW("Front Kick", "Strikes forward with a quick front kick.", CARD_ATTACK, STYLE_TAEKWONDO, 1, 12, 5, 1, 12),
    [CARD_TKD_SIDE_KICK] = CARD_ROW("Side Kick", "Drives the heel forward with powerful force.", CARD_ATTACK, STYLE_TAEKWONDO, 2, 20, 5, 1, 12),
    [CARD_TKD_ROUNDHOUSE_KICK] = CARD_ROW("Roundhouse Kick", "Swings the leg around to strike the opponent.", CARD_ATTACK, STYLE_TAEKWONDO, 2, 19, 5, 1, 12),
    [CARD_TKD_AXE_KICK] = CARD_ROW("Axe Kick", "Raises the leg and brings the heel downward.", CARD_ATTACK, STYLE_TAEKWONDO, 3, 27, 5, 1, 12),
    [CARD_TKD_BACK_KICK] = CARD_ROW("Back Kick", "Strikes backward with a powerful heel thrust.", CARD_ATTACK, STYLE_TAEKWONDO, 3, 25, 5, 1, 12),
    [CARD_TKD_SPINNING_HOOK_KICK] = CARD_ROW("Spinning Hook Kick", "Spins and whips the heel toward the opponent.", CARD_ATTACK, STYLE_TAEKWONDO, 4, 32, 5, 1, 12),
    [CARD_TKD_JUMPING_FRONT_KICK] = CARD_ROW("Jumping Front Kick", "Leaps forward and delivers a front kick.", CARD_ATTACK, STYLE_TAEKWONDO, 3, 23, 5, 1, 12),
    [CARD_TKD_DOUBLE_ROUNDHOUSE] = CARD_ROW("Double Roundhouse", "Delivers two consecutive roundhouse kicks.", CARD_ATTACK, STYLE_TAEKWONDO, 3, 30, 5, 1, 12),
    [CARD_TKD_TORNADO_KICK] = CARD_ROW("Tornado Kick", "Spins into a powerful jumping kick.", CARD_ATTACK, STYLE_TAEKWONDO, 5, 36, 5, 1, 12),
    [CARD_TKD_BACKSTEP_DODGE] = CARD_ROW("Backstep Dodge", "Steps backward to evade an incoming kick.", CARD_EVADE, STYLE_TAEKWONDO, 1, 0, 0, 1, 12),
    [CARD_TKD_HIGH_GUARD] = CARD_ROW("High Guard", "Raises the arms to defend against high strikes.", CARD_BLOCK, STYLE_TAEKWONDO, 1, 0, 0, 1, 12),
    [CARD_TKD_SIDE_STEP_GUARD] = CARD_ROW("Side-Step Guard", "Steps sideways to evade a spinning or straight kick.", CARD_EVADE, STYLE_TAEKWONDO, 2, 0, 0, 1, 12),

    // ---------- BRAZILIAN JIU-JITSU ----------
    [CARD_BJJ_SINGLE_LEG_TAKEDOWN] = CARD_ROW("Single-Leg Takedown", "Grabs one leg to bring the opponent to the ground.", CARD_ATTACK, STYLE_BJJ, 2, 12, 5, 1, 12),
    [CARD_BJJ_DOUBLE_LEG_TAKEDOWN] = CARD_ROW("Double-Leg Takedown", "Drives forward and takes the opponent down using both legs.", CARD_ATTACK, STYLE_BJJ, 3, 18, 5, 1, 12),
    [CARD_BJJ_GUARD_SWEEP] = CARD_ROW("Guard Sweep", "Reverses position to place the opponent underneath.", CARD_ATTACK, STYLE_BJJ, 2, 14, 5, 1, 12),
    [CARD_BJJ_ARMBAR] = CARD_ROW("Armbar", "Applies a controlled arm lock to force submission.", CARD_ATTACK, STYLE_BJJ, 3, 28, 5, 1, 12),
    [CARD_BJJ_TRIANGLE_CHOKE] = CARD_ROW("Triangle Choke", "Traps the opponent's head and arm with the legs.", CARD_ATTACK, STYLE_BJJ, 3, 24, 5, 1, 12),
    [CARD_BJJ_REAR_NAKED_CHOKE] = CARD_ROW("Rear-Naked Choke", "Applies a rear choke to force submission.", CARD_ATTACK, STYLE_BJJ, 4, 32, 5, 1, 12),
    [CARD_BJJ_KIMURA_LOCK] = CARD_ROW("Kimura Lock", "Uses a shoulder lock to control the opponent's arm.", CARD_ATTACK, STYLE_BJJ, 2, 22, 5, 1, 12),
    [CARD_BJJ_MOUNT_CONTROL] = CARD_ROW("Mount Control", "Secures a dominant position on top of the opponent.", CARD_ATTACK, STYLE_BJJ, 2, 10, 5, 1, 12),
    [CARD_BJJ_GROUND_AND_POUND] = CARD_ROW("Ground-and-Pound", "Delivers strikes while maintaining top control.", CARD_ATTACK, STYLE_BJJ, 2, 20, 5, 1, 12),
    [CARD_BJJ_SWEEP_ARMBAR_COMBO] = CARD_ROW("Sweep-Armbar Combo", "Reverses position before attempting an arm lock.", CARD_ATTACK, STYLE_BJJ, 4, 34, 5, 1, 12),
    [CARD_BJJ_GUARD_RETENTION_DODGE] = CARD_ROW("Guard Retention Dodge", "Repositions to avoid a takedown or submission attempt.", CARD_EVADE, STYLE_BJJ, 1, 0, 0, 1, 12),
    [CARD_BJJ_CLOSED_GUARD_BLOCK] = CARD_ROW("Closed Guard Block", "Uses a close guard to defend against ground attacks.", CARD_BLOCK, STYLE_BJJ, 1, 0, 0, 1, 12),
    [CARD_BJJ_SUBMISSION_ESCAPE] = CARD_ROW("Submission Escape", "Breaks free from an opponent's submission hold.", CARD_EVADE, STYLE_BJJ, 2, 0, 0, 1, 12),
    [CARD_BJJ_SPRAWL] = CARD_ROW("Sprawl", "Throws the legs back to stop a takedown attempt.", CARD_BLOCK, STYLE_BJJ, 2, 0, 0, 1, 12),

    // ---------- KARATE ----------
    [CARD_KAR_STRAIGHT_PUNCH] = CARD_ROW("Straight Punch", "Delivers a direct punch with speed and precision.", CARD_ATTACK, STYLE_KARATE, 1, 14, 5, 1, 12),
    [CARD_KAR_REVERSE_PUNCH] = CARD_ROW("Reverse Punch", "Drives a powerful straight punch from the rear hand.", CARD_ATTACK, STYLE_KARATE, 2, 21, 5, 1, 12),
    [CARD_KAR_KNIFE_HAND_STRIKE] = CARD_ROW("Knife-Hand Strike", "Strikes with the edge of the open hand.", CARD_ATTACK, STYLE_KARATE, 2, 20, 5, 1, 12),
    [CARD_KAR_FRONT_KICK] = CARD_ROW("Front Kick", "Pushes forward with a direct front kick.", CARD_ATTACK, STYLE_KARATE, 2, 16, 5, 1, 12),
    [CARD_KAR_SIDE_KICK] = CARD_ROW("Side Kick", "Delivers a forceful sideways kick with the heel.", CARD_ATTACK, STYLE_KARATE, 2, 22, 5, 1, 12),
    [CARD_KAR_ROUNDHOUSE_KICK] = CARD_ROW("Roundhouse Kick", "Swings the leg in an arc toward the opponent.", CARD_ATTACK, STYLE_KARATE, 3, 24, 5, 1, 12),
    [CARD_KAR_BACKFIST] = CARD_ROW("Backfist", "Snaps the back of the fist toward the target.", CARD_ATTACK, STYLE_KARATE, 1, 12, 5, 1, 12),
    [CARD_KAR_SPINNING_BACK_KICK] = CARD_ROW("Spinning Back Kick", "Spins around and drives the heel backward.", CARD_ATTACK, STYLE_KARATE, 4, 31, 5, 1, 12),
    [CARD_KAR_KARATE_COMBINATION] = CARD_ROW("Karate Combination", "Chains three quick strikes into one attack.", CARD_ATTACK, STYLE_KARATE, 3, 30, 5, 1, 12),
    [CARD_KAR_COUNTER_STRIKE] = CARD_ROW("Counter Strike", "Punishes the opponent immediately after a missed attack.", CARD_ATTACK, STYLE_KARATE, 2, 24, 5, 1, 12),
    [CARD_KAR_TAI_SABAKI_DODGE] = CARD_ROW("Tai Sabaki Dodge", "Shifts the body off-line to evade an incoming strike.", CARD_EVADE, STYLE_KARATE, 1, 0, 0, 1, 12),
    [CARD_KAR_KARATE_GUARD] = CARD_ROW("Karate Guard", "Maintains a defensive stance to absorb incoming strikes.", CARD_BLOCK, STYLE_KARATE, 1, 0, 0, 1, 12),
    [CARD_KAR_COUNTER_BLOCK] = CARD_ROW("Counter Block", "Deflects an attack and prepares a quick counterstrike.", CARD_BLOCK, STYLE_KARATE, 2, 0, 0, 1, 12)
};

const Card *card_get(uint8_t id)
{
    if (id >= CARD_COUNT) {
        return NULL;
    }

    return &CARD_TABLE[id];
}

bool card_table_valid(void)
{
    for (int i = 0; i < CARD_COUNT; i++) {
        if (CARD_TABLE[i].name[0] == '\0') return false;   // forgotten entry
        if (CARD_TABLE[i].dice_min > CARD_TABLE[i].dice_max) return false;
    }
    return true;
}

int card_pool_for_style(FightingStyle style, uint8_t *out, int max)
{
    if (out == NULL || max <= 0) {
        return 0;
    }

    if (style < STYLE_BOXING || style >= STYLE_COUNT) {
        return 0;
    }

    int count = 0;

    for (int i = 0; i < CARD_COUNT && count < max; i++) {
        if (CARD_TABLE[i].style == (uint8_t)style) {
            out[count++] = (uint8_t)i;
        }
    }

    return count;
}

// Adds copies of a card ID to the output deck.
static int add_copies(
    uint8_t *out,
    int count,
    int max,
    uint8_t id,
    int copies
)
{
    for (int i = 0; i < copies && count < max; i++) {
        out[count++] = id;
    }

    return count;
}

// Starter decks contain 11 cards:
// 4 basic attacks, 4 defensive cards, 2 additional attacks, 1 evade.
int get_starter_deck(FightingStyle style, uint8_t *out, int max)
{
    if (out == NULL || max <= 0) {
        return 0;
    }

    if (style < STYLE_BOXING || style >= STYLE_COUNT) {
        return 0;
    }

    int count = 0;

    switch (style) {
        case STYLE_BOXING:
            count = add_copies(out, count, max, CARD_BOX_JAB, 4);
            count = add_copies(out, count, max, CARD_BOX_BOXING_GUARD, 4);
            count = add_copies(out, count, max, CARD_BOX_CROSS, 1);
            count = add_copies(out, count, max, CARD_BOX_BODY_SHOT, 1);
            count = add_copies(out, count, max, CARD_BOX_SLIP_DODGE, 1);
            break;

        case STYLE_MUAY_THAI:
            count = add_copies(out, count, max, CARD_MT_TEEP, 4);
            count = add_copies(out, count, max, CARD_MT_LEG_CHECK, 4);
            count = add_copies(out, count, max, CARD_MT_LOW_KICK, 1);
            count = add_copies(out, count, max, CARD_MT_KNEE_STRIKE, 1);
            count = add_copies(out, count, max, CARD_MT_SWAY_DODGE, 1);
            break;

        case STYLE_TAEKWONDO:
            count = add_copies(out, count, max, CARD_TKD_FRONT_KICK, 4);
            count = add_copies(out, count, max, CARD_TKD_HIGH_GUARD, 4);
            count = add_copies(out, count, max, CARD_TKD_SIDE_KICK, 1);
            count = add_copies(out, count, max, CARD_TKD_ROUNDHOUSE_KICK, 1);
            count = add_copies(out, count, max, CARD_TKD_BACKSTEP_DODGE, 1);
            break;

        case STYLE_BJJ:
            count = add_copies(out, count, max, CARD_BJJ_SINGLE_LEG_TAKEDOWN, 4);
            count = add_copies(out, count, max, CARD_BJJ_CLOSED_GUARD_BLOCK, 4);
            count = add_copies(out, count, max, CARD_BJJ_GUARD_SWEEP, 1);
            count = add_copies(out, count, max, CARD_BJJ_KIMURA_LOCK, 1);
            count = add_copies(out, count, max, CARD_BJJ_GUARD_RETENTION_DODGE, 1);
            break;

        case STYLE_KARATE:
            count = add_copies(out, count, max, CARD_KAR_STRAIGHT_PUNCH, 4);
            count = add_copies(out, count, max, CARD_KAR_KARATE_GUARD, 4);
            count = add_copies(out, count, max, CARD_KAR_REVERSE_PUNCH, 1);
            count = add_copies(out, count, max, CARD_KAR_SIDE_KICK, 1);
            count = add_copies(out, count, max, CARD_KAR_TAI_SABAKI_DODGE, 1);
            break;

        default:
            return 0;
    }

    return count;
}

int card_roll_dice(const Card *c)
{
    if (c == NULL || c->dice_min > c->dice_max) {
        return 0;
    }

    return roll_range(c->dice_min, c->dice_max);
}

int card_calculate_damage(const Card *c, int roll)
{
    if (c == NULL) {
        return 0;
    }

    if (c->type != CARD_ATTACK) {
        return 0;
    }

    if (roll < c->dice_min || roll > c->dice_max) {
        return 0;
    }

    return (int)c->damage + roll;
}
