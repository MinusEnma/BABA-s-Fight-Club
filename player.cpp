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

void Player::buildDeck() {
    deck.clear();
    hand.clear();
    discardPile.clear();
    comboQueue.clear();
 
    vector<Card> pool;
    switch (style) {
        case FightingStyle::BOXING:    pool = getBoxingCards();    break;
        case FightingStyle::MUAY_THAI: pool = getMuayThaiCards();  break;
        case FightingStyle::TAEKWONDO: pool = getTaekwondoCards(); break;
        case FightingStyle::BJJ:       pool = getBJJCards();       break;
        case FightingStyle::KARATE:    pool = getKarateCards();    break;
    }
 
    for (const Card& c : pool) {
        deck.push_back(c);
    }
 
    shuffleDeck();
}

void Player::shuffleDeck() {
    shuffle(deck.begin(), deck.end(), rng);
}

bool Player::drawCard() {
    // Draw pile empty? Shuffle the discard pile back into it
    if (deck.empty()) {
        if (discardPile.empty()) return false;   // nothing left anywhere
        deck = discardPile;
        discardPile.clear();
        shuffleDeck();
    }
 
    hand.push_back(deck.back());
    deck.pop_back();
    return true;
}

void Player::drawhand() {
    while ((int)hand.size() < HAND_SIZE) {
        if (!drawCard()) break;
    }
}
 
void Player::discardHand() {
    for (const Card& c : hand) {
        discardPile.push_back(c);
    }
    hand.clear();
}

int Player::rollInitiative() const { // rolls a d12 and adds the player's speed stat
    return randomBetween(1, 12) + speed;
}
 
bool Player::queueCombo(int handIndex) { // puts cards in queue
    if (handIndex < 0 || handIndex >= (int)hand.size()) return false;
 
    const Card& c = hand[handIndex];
    if (c.getEnergyCost() > energy) return false;   // not enough energy
 
    energy -= c.getEnergyCost();
    comboQueue.push_back(c);
    hand.erase(hand.begin() + handIndex);
    return true;
}

void Player::clearQueue() { // Played cards go to the discard pile
    for (const Card& c : comboQueue) {
        discardPile.push_back(c);
    }
    comboQueue.clear();
}

/* 
    Combat stuff
*/

int Player::rollDefense() {
    return randomBetween(1, 6) + stats.getDefense() + stats.getDefenseRollBonus();
}
 
void Player::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
}
 
void Player::takeStaggerDamage(int amount) {
    staggerHealth -= amount;
    if (staggerHealth <= 0) {
        staggerHealth = 0;
        stunned = true;
        clearQueue();   // played cards are nullified and go to the discard pile
    }
}
 
bool Player::isStunned() const {
    return stunned;
}
