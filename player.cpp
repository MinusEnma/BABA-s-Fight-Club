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


