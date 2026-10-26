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

// getters

string Player::getName() const {
	return this->name;
}

FightingStyle Player::getStyle() const {
	return this->style;
}

int Player::getHp() const {
	return this->hp;
}

int Player::getStagger() const {
	return this->staggerHealth;
}

int Player::getEnergy() const {
	return this->energy;
}

const vector<Card>& Player::getHand() const {
	return this->hand;
}

const vector<Card>& Player::getComboQueue() const {
	return this->comboQueue;
}
