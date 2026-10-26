#pragma once
#include <string>
#include <vector>
#include "card.h"
#include "cardDatabase.h"
using namespace std;
 
class Player {
    private:
        static const int HAND_SIZE = 6;
 
        string name;
        FightingStyle style;
        StyleStats stats;
 
        int hp, maxHp, staggerHealth, energy, defense, speed;
        bool hasInitiative, stunned;
 
        vector<Card> hand;
        vector<Card> deck;
        vector<Card> discardPile;
        vector<Card> comboQueue;

    public:
        Player();
        Player(string name, FightingStyle style);

        void buildDeck();
        void drawhand();
        void shuffleDeck();

        int rollInitiative() const;
        bool queueCombo(int handIndex);
        void clearQueue();

        bool drawCard();            // draw 1 card (reshuffles discard if deck is empty)
        void discardHand();         // move the whole hand to the discard pile
 
        int rollDefense();
        void takeDamage(int amount);
        void takeStaggerDamage(int amount);
        bool isStunned() const;
 
        void resetRound();
 
        string getName() const;
        FightingStyle getStyle() const;
        int getHp() const;
        int getStagger() const;
        int getEnergy() const;
        int getDeckSize() const;
        int getDiscardSize() const;
        const vector<Card>& getHand() const;
        const vector<Card>& getComboQueue() const;
};
