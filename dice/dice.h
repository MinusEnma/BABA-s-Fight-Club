#ifndef DICE_H
#define DICE_H

// Call once at program start.
void dice_seed(void);

// General dice functions
int roll(int sides);
int roll_range(int lo, int hi);

// Dedicated dice functions
int roll_initiative(void);  // d20

#endif

