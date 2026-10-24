#include "player.h"
#include <algorithm>
#include <random>
using namespace std;
 
static mt19937 rng(random_device{}());
 
static int randomBetween(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

Player::Player() {
    name = "Unknown";
    style = FightingStyle::BOXING;      
    stats = StyleStats(5, 5, 0, 0);
 
    hp = maxHp = 250;
    staggerHealth = 125;
    energy = 0;
    defense = stats.getDefense();
    speed = stats.getSpeed();
    hasInitiative = false;
    stunned = false;
}
 
Player::Player(string name, FightingStyle style) {
    this->name = name;
    this->style = style;
    stats = getStyleStats(style);       
 
    hp = maxHp = 250;                   
    energy = 0;
    defense = stats.getDefense();
    speed = stats.getSpeed();
    hasInitiative = false;
    stunned = false;
}