#pragma once
#include <string>
using namespace std;

enum class FightingStyle {
    BOXING, MUAY_THAI, TAEKWONDO, BJJ, KARATE
};

enum class CardType {
    ATTACK, BLOCK, EVADE
};

class Card {
    private:
        string name, description;
        CardType type;
        FightingStyle style;

        int energyCost, damage, staggerDamage, diceMin, diceMax, copies;

    public:
        Card();
        Card(string name, string description, CardType type, FightingStyle style, int energyCost, int damage, int staggerDamage, int diceMin, int diceMax);
        int rollDice() const;
        int calculateDamage(int roll) const;
        string getName() const;
        string getDescription() const;
        CardType getType() const;
        FightingStyle getStyle() const;
        int baseDamage() const;
        int getEnergyCost() const;
        int getStaggerDamage() const;
        int getDiceMin() const;
        int getDiceMax() const;
        int getCopies() const;
};