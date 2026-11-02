#ifndef DICE_H
#define DICE_H

// Call once at program start (the server only, if the server does all the rolling)
void dice_seed(void);

int roll(int sides);                 // 1 .. sides
int roll_range(int lo, int hi);      // lo .. hi (inclusive)

int roll_initiative(void);           // d20
int roll_attack(void);               // d12

#endif