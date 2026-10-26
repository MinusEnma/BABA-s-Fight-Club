#include "cardDatabase.h"
using namespace std;

// (energyCost, damage, staggerDamage, diceMin, diceMax)

vector<Card> getBoxingCards(){
    vector<Card> boxingCards;
    boxingCards.push_back(Card("Jab", "A quick punch to the opponent", CardType::ATTACK, FightingStyle::BOXING, 1, 10, 5, 1, 12));
    boxingCards.push_back(Card("Cross", "A powerful, straight punch delivered with the rear hand.", CardType::ATTACK, FightingStyle::BOXING, 2, 18, 5, 1, 12));
    boxingCards.push_back(Card("Lead Hook", "A curved punch targeting the side of the opponent.", CardType::ATTACK, FightingStyle::BOXING, 3, 25, 5, 1, 12));
    boxingCards.push_back(Card("Uppercut", "An upward punch aimed at the opponent’s chin.", CardType::ATTACK, FightingStyle::BOXING, 3, 25, 5, 1, 12));
    boxingCards.push_back(Card("Body Shot", "A punch aimed at the opponent’s torso.", CardType::ATTACK, FightingStyle::BOXING, 2, 16, 5, 1, 12));
    boxingCards.push_back(Card("Jab-Cross", "Combines a quick jab with a powerful cross.", CardType::ATTACK, FightingStyle::BOXING, 3, 28, 5, 1, 12));
    boxingCards.push_back(Card("Hook-Cross", "Delivers a hook followed by a straight cross.", CardType::ATTACK, FightingStyle::BOXING, 4, 35, 5, 1, 12));
    boxingCards.push_back(Card("Counter Punch", "Strikes immediately after the opponent misses.", CardType::ATTACK, FightingStyle::BOXING, 2, 18, 5, 1, 12));
    boxingCards.push_back(Card("Slip Dodge", "Quickly moves the head aside to evade a punch.", CardType::EVADE, FightingStyle::BOXING, 1, 0, 0, 1, 12));
    boxingCards.push_back(Card("Boxing Guard", "Raises the arms to absorb incoming damage.", CardType::EVADE, FightingStyle::BOXING, 1, 0, 0, 1, 12));
    boxingCards.push_back(Card("Parry", "Deflects an incoming punch with precise timing.", CardType::BLOCK, FightingStyle::BOXING, 2, 0, 0, 1, 12));
    return boxingCards;
}

vector<Card> getMuayThaiCards(){
    vector<Card> muayThaiCards;
    muayThaiCards.push_back(Card("Teep", "Pushes the opponent back with a straight front kick.", CardType::ATTACK, FightingStyle::MUAY_THAI, 1, 12, 5, 1, 12));
    muayThaiCards.push_back(Card("Roundhouse Kick", "Swings the leg in a powerful circular kick.", CardType::ATTACK, FightingStyle::MUAY_THAI, 3, 24, 5, 1, 12));
    muayThaiCards.push_back(Card("Low Kick", "Targets the opponent's thigh with a low kick.", CardType::ATTACK, FightingStyle::MUAY_THAI, 2, 16, 5, 1, 12));
    muayThaiCards.push_back(Card("Body Kick", "Delivers a powerful kick to the torso.", CardType::ATTACK, FightingStyle::MUAY_THAI, 2, 22, 5, 1, 12));
    muayThaiCards.push_back(Card("Knee Strike", "Drives the knee forward into the opponent.", CardType::ATTACK, FightingStyle::MUAY_THAI, 2, 23, 5, 1, 12));
    muayThaiCards.push_back(Card("Elbow Strike", "Delivers a sharp, close-range elbow strike.", CardType::ATTACK, FightingStyle::MUAY_THAI, 3, 26, 5, 1, 12));
    muayThaiCards.push_back(Card("Clinch Knee Combo", "Controls the opponent before landing two knee strikes.", CardType::ATTACK, FightingStyle::MUAY_THAI, 4, 32, 5, 1, 12));
    muayThaiCards.push_back(Card("Spinning Elbow", "Rotates quickly to deliver a powerful elbow strike.", CardType::ATTACK, FightingStyle::MUAY_THAI, 4, 34, 5, 1, 12));
    muayThaiCards.push_back(Card("Kick-Knee Combo", "Follows a kick with a driving knee strike.", CardType::ATTACK, FightingStyle::MUAY_THAI, 4, 35, 5, 1, 12));
    muayThaiCards.push_back(Card("Sway Dodge", "Leans away to evade an incoming close-range strike.", CardType::EVADE, FightingStyle::MUAY_THAI, 1, 0, 0, 1, 12));
    muayThaiCards.push_back(Card("Thai Guard", "Uses a tight guard to absorb incoming strikes.", CardType::BLOCK, FightingStyle::MUAY_THAI, 2, 0, 0, 1, 12));
    muayThaiCards.push_back(Card("Leg Check", "Raises the leg to defend against an incoming kick.", CardType::BLOCK, FightingStyle::MUAY_THAI, 1, 0, 0, 1, 12));
    return muayThaiCards;
}

vector<Card> getTaekwondoCards(){
    vector<Card> taekwondoCards;
    taekwondoCards.push_back(Card("Front Kick", "Strikes forward with a quick front kick.", CardType::ATTACK, FightingStyle::TAEKWONDO, 1, 12, 5, 1, 12));
    taekwondoCards.push_back(Card("Side Kick", "Drives the heel forward with powerful force.", CardType::ATTACK, FightingStyle::TAEKWONDO, 2, 20, 5, 1, 12));
    taekwondoCards.push_back(Card("Roundhouse Kick", "Swings the leg around to strike the opponent.", CardType::ATTACK, FightingStyle::TAEKWONDO, 2, 19, 5, 1, 12));
    taekwondoCards.push_back(Card("Axe Kick", "Raises the leg and brings the heel downward.", CardType::ATTACK, FightingStyle::TAEKWONDO, 3, 27, 5, 1, 12));
    taekwondoCards.push_back(Card("Back Kick", "Strikes backward with a powerful heel thrust.", CardType::ATTACK, FightingStyle::TAEKWONDO, 3, 25, 5, 1, 12));
    taekwondoCards.push_back(Card("Spinning Hook Kick", "Spins and whips the heel toward the opponent.", CardType::ATTACK, FightingStyle::TAEKWONDO, 4, 32, 5, 1, 12));
    taekwondoCards.push_back(Card("Jumping Front Kick", "Leaps forward and delivers a front kick.", CardType::ATTACK, FightingStyle::TAEKWONDO, 3, 23, 5, 1, 12));
    taekwondoCards.push_back(Card("Double Roundhouse", "Delivers two consecutive roundhouse kicks.", CardType::ATTACK, FightingStyle::TAEKWONDO, 3, 30, 5, 1, 12));
    taekwondoCards.push_back(Card("Tornado Kick", "Spins into a powerful jumping kick.", CardType::ATTACK, FightingStyle::TAEKWONDO, 5, 36, 5, 1, 12));
    taekwondoCards.push_back(Card("Backstep Dodge", "Steps backward to evade an incoming kick.", CardType::EVADE, FightingStyle::TAEKWONDO, 1, 0, 0, 1, 12));
    taekwondoCards.push_back(Card("High Guard", "Raises the arms to defend against high strikes.", CardType::BLOCK, FightingStyle::TAEKWONDO, 1, 0, 0, 1, 12));
    taekwondoCards.push_back(Card("Side-Step Guard", "Steps sideways to evade a spinning or straight kick.", CardType::EVADE, FightingStyle::TAEKWONDO, 2, 0, 0, 1, 12));
    return taekwondoCards;
}

vector<Card> getBJJCards(){
    vector<Card> bjjCards;
    bjjCards.push_back(Card("Single-Leg Takedown", "Grabs one leg to bring the opponent to the ground.", CardType::ATTACK, FightingStyle::BJJ, 2, 12, 5, 1, 12));
    bjjCards.push_back(Card("Double-Leg Takedown", "Drives forward and takes the opponent down using both legs.", CardType::ATTACK, FightingStyle::BJJ, 3, 18, 5, 1, 12));
    bjjCards.push_back(Card("Guard Sweep", "Reverses position to place the opponent underneath.", CardType::ATTACK, FightingStyle::BJJ, 2, 14, 5, 1, 12));
    bjjCards.push_back(Card("Armbar", "Applies a controlled arm lock to force submission.", CardType::ATTACK, FightingStyle::BJJ, 3, 28, 5, 1, 12));
    bjjCards.push_back(Card("Triangle Choke", "Traps the opponent's head and arm with the legs.", CardType::ATTACK, FightingStyle::BJJ, 3, 24, 5, 1, 12));
    bjjCards.push_back(Card("Rear-Naked Choke", "Applies a rear choke to force submission.", CardType::ATTACK, FightingStyle::BJJ, 4, 32, 5, 1, 12));
    bjjCards.push_back(Card("Kimura Lock", "Uses a shoulder lock to control the opponent's arm.", CardType::ATTACK, FightingStyle::BJJ, 2, 22, 5, 1, 12));
    bjjCards.push_back(Card("Mount Control", "Secures a dominant position on top of the opponent.", CardType::ATTACK, FightingStyle::BJJ, 2, 10, 5, 1, 12));
    bjjCards.push_back(Card("Ground-and-Pound", "Delivers strikes while maintaining top control.", CardType::ATTACK, FightingStyle::BJJ, 2, 20, 5, 1, 12));
    bjjCards.push_back(Card("Sweep-Armbar Combo", "Reverses position before attempting an arm lock.", CardType::ATTACK, FightingStyle::BJJ, 4, 34, 5, 1, 12));
    bjjCards.push_back(Card("Guard Retention Dodge", "Repositions to avoid a takedown or submission attempt.", CardType::EVADE, FightingStyle::BJJ, 1, 0, 0, 1, 12));
    bjjCards.push_back(Card("Closed Guard Block", "Uses a close guard to defend against ground attacks.", CardType::BLOCK, FightingStyle::BJJ, 1, 0, 0, 1, 12));
    bjjCards.push_back(Card("Submission Escape", "Breaks free from an opponent's submission hold.", CardType::EVADE, FightingStyle::BJJ, 2, 0, 0, 1, 12));
    bjjCards.push_back(Card("Sprawl", "Throws the legs back to stop a takedown attempt.", CardType::BLOCK, FightingStyle::BJJ, 2, 0, 0, 1, 12));
    return bjjCards;
}
 
vector<Card> getKarateCards(){
    vector<Card> karateCards;
    karateCards.push_back(Card("Straight Punch", "Delivers a direct punch with speed and precision.", CardType::ATTACK, FightingStyle::KARATE, 1, 14, 5, 1, 12));
    karateCards.push_back(Card("Reverse Punch", "Drives a powerful straight punch from the rear hand.", CardType::ATTACK, FightingStyle::KARATE, 2, 21, 5, 1, 12));
    karateCards.push_back(Card("Knife-Hand Strike", "Strikes with the edge of the open hand.", CardType::ATTACK, FightingStyle::KARATE, 2, 20, 5, 1, 12));
    karateCards.push_back(Card("Front Kick", "Pushes forward with a direct front kick.", CardType::ATTACK, FightingStyle::KARATE, 2, 16, 5, 1, 12));
    karateCards.push_back(Card("Side Kick", "Delivers a forceful sideways kick with the heel.", CardType::ATTACK, FightingStyle::KARATE, 2, 22, 5, 1, 12));
    karateCards.push_back(Card("Roundhouse Kick", "Swings the leg in an arc toward the opponent.", CardType::ATTACK, FightingStyle::KARATE, 3, 24, 5, 1, 12));
    karateCards.push_back(Card("Backfist", "Snaps the back of the fist toward the target.", CardType::ATTACK, FightingStyle::KARATE, 1, 12, 5, 1, 12));
    karateCards.push_back(Card("Spinning Back Kick", "Spins around and drives the heel backward.", CardType::ATTACK, FightingStyle::KARATE, 4, 31, 5, 1, 12));
    karateCards.push_back(Card("Karate Combination", "Chains three quick strikes into one attack.", CardType::ATTACK, FightingStyle::KARATE, 3, 30, 5, 1, 12));
    karateCards.push_back(Card("Counter Strike", "Punishes the opponent immediately after a missed attack.", CardType::ATTACK, FightingStyle::KARATE, 2, 24, 5, 1, 12));
    karateCards.push_back(Card("Tai Sabaki Dodge", "Shifts the body off-line to evade an incoming strike.", CardType::EVADE, FightingStyle::KARATE, 1, 0, 0, 1, 12));
    karateCards.push_back(Card("Karate Guard", "Maintains a defensive stance to absorb incoming strikes.", CardType::BLOCK, FightingStyle::KARATE, 1, 0, 0, 1, 12));
    karateCards.push_back(Card("Counter Block", "Deflects an attack and prepares a quick counterstrike.", CardType::BLOCK, FightingStyle::KARATE, 2, 0, 0, 1, 12));
    return karateCards;
}

StyleStats getStyleStats(FightingStyle style) {
    switch (style) {
        // (def spd defBonus evadeBonus)
        case FightingStyle::BOXING:    return StyleStats(4, 7, 0, 1);
        case FightingStyle::MUAY_THAI: return StyleStats(6, 5, 1, 0);
        case FightingStyle::TAEKWONDO: return StyleStats(3, 8, 0, 2);
        case FightingStyle::BJJ:       return StyleStats(7, 3, 2, 0);
        case FightingStyle::KARATE:    return StyleStats(5, 6, 3, 1);
    }
    return StyleStats(5, 5, 0, 0);
}

static void addCopies(vector<Card>& deck, const vector<Card>& pool, const string& cardName, int copies) {
    for (const Card& c : pool) {
        if (c.getName() == cardName) {
            for (int i = 0; i < copies; i++) {
                deck.push_back(c);
            }
            return;
        }
    }
}
 
vector<Card> getStarterDeck(FightingStyle style) {
    vector<Card> deck;
    vector<Card> pool;
    //(amount of copied in each character's starter deck)
    switch (style) {
        case FightingStyle::BOXING:
            pool = getBoxingCards();
            addCopies(deck, pool, "Jab", 4);
            addCopies(deck, pool, "Boxing Guard", 4);
            addCopies(deck, pool, "Cross", 1);
            addCopies(deck, pool, "Body Shot", 1);
            addCopies(deck, pool, "Slip Dodge", 1);
            break;
 
        case FightingStyle::MUAY_THAI:
            pool = getMuayThaiCards();
            addCopies(deck, pool, "Teep", 4);
            addCopies(deck, pool, "Leg Check", 4);
            addCopies(deck, pool, "Low Kick", 1);
            addCopies(deck, pool, "Knee Strike", 1);
            addCopies(deck, pool, "Sway Dodge", 1);
            break;
 
        case FightingStyle::TAEKWONDO:
            pool = getTaekwondoCards();
            addCopies(deck, pool, "Front Kick", 4);
            addCopies(deck, pool, "High Guard", 4);
            addCopies(deck, pool, "Side Kick", 1);
            addCopies(deck, pool, "Roundhouse Kick", 1);
            addCopies(deck, pool, "Backstep Dodge", 1);
            break;
 
        case FightingStyle::BJJ:
            pool = getBJJCards();
            addCopies(deck, pool, "Single-Leg Takedown", 4);   // BJJ has no 1-cost attack
            addCopies(deck, pool, "Closed Guard Block", 4);
            addCopies(deck, pool, "Guard Sweep", 1);
            addCopies(deck, pool, "Kimura Lock", 1);
            addCopies(deck, pool, "Guard Retention Dodge", 1);
            break;
 
        case FightingStyle::KARATE:
            pool = getKarateCards();
            addCopies(deck, pool, "Straight Punch", 4);
            addCopies(deck, pool, "Karate Guard", 4);
            addCopies(deck, pool, "Reverse Punch", 1);
            addCopies(deck, pool, "Side Kick", 1);
            addCopies(deck, pool, "Tai Sabaki Dodge", 1);
            break;
    }
 
    return deck;
}