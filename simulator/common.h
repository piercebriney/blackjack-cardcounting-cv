#ifndef COMMON
#define COMMON
#include "params.h"
#include <vector>
#include <string>
#include "pcg_random.hpp"
#include <array>

    
typedef pcg64_fast Rng;
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

enum action {hit, stay, split, doubledown, surrender, voidaction};

extern std::vector<std::array<int, 10>>g_countingMethods;

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
  stack dealersCards; //player can only see dealersCards[0]
  
  std::vector<stack> perceivedStacks; //these ones are filled by the player by perception
  stack dealersPerceivedCards;

  gamestate() {
  }

  void clear() {
      stacks.clear();
      actions.clear();
      dealersCards.clear();
      perceivedStacks.clear();
      dealersPerceivedCards.clear();
  }
};

struct deviation {
  int total; //the player's sum
  stack hand; //the player's hand
  bool useHandNotTotal = 0; //set to 1 to use hand instead of total
  effectiveCard dealerUpCard;
  int index; //if TC is above index, use result instead of BS
  action result;
  bool activeLessThan = 0; //activate if it's tc is less than index
};

void printPlayerCards(gamestate& g, int stackIndex);

void printStack(stack& s);

bool isBlackjack(card a, card b);

bool areStacksEqual(stack& a, stack& b);

bool areStacksEffectivelyEqual(stack& a, stack& b);

//count cards so that they are as high as possible without going over 21
int getIdealCount(stack& s);

card getRandomCard();

int getLowSumOfStack(stack& a);

#endif
