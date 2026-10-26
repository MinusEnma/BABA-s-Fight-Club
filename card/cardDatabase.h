#pragma once
#include <vector>
#include "card.h"
using namespace std;

class StyleStats {
    private:
        int defense;
        int speed;
        int defenseRollBonus;   
        int evadeBonus;         
    public:
        StyleStats() : defense(5), speed(5), defenseRollBonus(0), evadeBonus(0) {}
        StyleStats(int defense, int speed, int defenseRollBonus, int evadeBonus)
            : defense(defense), speed(speed),
              defenseRollBonus(defenseRollBonus), evadeBonus(evadeBonus) {}
 
        int getDefense() const { return defense; }
        int getSpeed() const { return speed; }
        int getDefenseRollBonus() const { return defenseRollBonus; }
        int getEvadeBonus() const { return evadeBonus; }
};
 
StyleStats getStyleStats(FightingStyle style);
 
vector<Card> getBoxingCards();
vector<Card> getMuayThaiCards();
vector<Card> getTaekwondoCards();
vector<Card> getBJJCards();
vector<Card> getKarateCards();
vector<Card> getStarterDeck(FightingStyle style);
