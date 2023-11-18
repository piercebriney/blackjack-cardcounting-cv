#ifndef COMMON
#define COMMON
#include "params.h"
#include <vector>
#include <string>

// print value
#define PRINT(a)                          \
{                                         \
  std::cout << #a << ": " << (a) << endl; \
}

enum card {_2S,_3S,_4S,_5S,_6S,_7S,_8S,_9S,_10S,_JS,_QS,_KS,_AS,   
                       _2H,_3H,_4H,_5H,_6H,_7H,_8H,_9H,_10H,_JH,_QH,_KH,_AH,
                       _2C,_3C,_4C,_5C,_6C,_7C,_8C,_9C,_10C,_JC,_QC,_KC,_AC,
                       _2D,_3D,_4D,_5D,_6D,_7D,_8D,_9D,_10D,_JD,_QD,_KD,_AD};

enum effectiveCard {_A, _2, _3, _4, _5, _6, _7, _8, _9, _T};

enum action {hit, stay, split, doubledown, surrender};

extern std::vector<std::vector<int>>g_countingMethods;

enum countingMethod {HiLo, HiOpt2};

void commonInit();

std::string getCardName(card a);

float getCountFromCard(card a, countingMethod b);

effectiveCard getEffectiveCard(card a);

std::string getEffectiveCardName(effectiveCard a);

int getEffectiveCardValue(effectiveCard a);

int getEffectiveCardValue(card a);

typedef std::vector<card> stack;

struct gamestate {
  std::vector<stack> stacks; //when you split, the player changes one stack into two stacks
  std::vector<action> actions; //the action the player has set for each stack- they can surrender on one and not on another, maybe
  std::vector<card> dealersCards; //player can only see dealersCards[0]
};

void printPlayerCards(gamestate g, int stackIndex);

bool isBlackjack(card a, card b);

#endif
