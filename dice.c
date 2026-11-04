#include <stdlib.h>
#include <time.h>
#include "dice.h"

void dice_seed(void) {
    srand(time(NULL));
}

int roll(int sides) {
    return (rand() % sides) + 1;
}

int roll_range(int lo, int hi) {
    return lo + (rand() % (hi - lo + 1));
}

int roll_initiative(void) {
    return roll(20);
}

int roll_attack(void) {
    return roll(12);
}

