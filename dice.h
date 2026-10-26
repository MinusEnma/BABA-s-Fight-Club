#pragma once
#include <random>

int roll(int sides) {
    static std::random_device seedSource;   // Asks the computer for a random starting number
    static std::mt19937 gen(seedSource());  // The dice machine, started with that number

    std::uniform_int_distribution<int> dist(1, sides); // Picks a fair number from 1 to sides
    return dist(gen);                                  // Roll the dice and give back the result
}

int rollInitiative() {
    return roll(20); // Roll a d20 for initiative
}

int rollAttack() {
    return roll(12); // Roll a d12 for attack
}