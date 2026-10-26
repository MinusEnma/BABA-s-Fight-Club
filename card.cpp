#include "card.h"
#include <random>
#include <utility>

Card::Card()
    : type(CardType::ATTACK), style(FightingStyle::BOXING), energyCost(0),
      damage(0), staggerDamage(0), diceMin(1), diceMax(1), copies(1) {}

Card::Card(string name, string description, CardType type, FightingStyle style,
           int energyCost, int damage, int staggerDamage, int diceMin, int diceMax)
    : name(std::move(name)), description(std::move(description)), type(type),
      style(style), energyCost(energyCost), damage(damage),
      staggerDamage(staggerDamage), diceMin(diceMin), diceMax(diceMax), copies(1) {}

int Card::rollDice() const {
    static thread_local std::mt19937 rng(std::random_device{}());
    return std::uniform_int_distribution<int>(diceMin, diceMax)(rng);
}

int Card::calculateDamage(int roll) const { return damage + roll; }
string Card::getName() const { return name; }
string Card::getDescription() const { return description; }
CardType Card::getType() const { return type; }
FightingStyle Card::getStyle() const { return style; }
int Card::baseDamage() const { return damage; }
int Card::getEnergyCost() const { return energyCost; }
int Card::getStaggerDamage() const { return staggerDamage; }
int Card::getDiceMin() const { return diceMin; }
int Card::getDiceMax() const { return diceMax; }
int Card::getCopies() const { return copies; }
