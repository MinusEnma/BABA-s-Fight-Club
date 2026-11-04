#include <stdlib.h>
#include <time.h>
#include "dice.h"

void dice_seed(void) {
    srand((unsigned int)time(NULL));
}

int roll(int sides) {
    if (sides <= 0) {
        return 0;
    }

    return (rand() % sides) + 1;
}

int roll_range(int lo, int hi) {
    if (lo > hi) {
        return 0;
    }

    long long range = (long long)hi - lo + 1;

    // Prevent invalid or excessively large ranges.
    if (range > (long long)RAND_MAX) {
        return 0;
    }

    return lo + (rand() % (int)range);
}

int roll_initiative(void) {
    return roll(20);
}

int roll_attack(void) {
    return roll(12);
}