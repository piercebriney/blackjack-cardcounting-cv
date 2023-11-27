#ifndef PLAYER
#define PLAYER

#include "matrix.h"
#include "common.h"

//this class is meant to view cards, keep a running count, and output info about hitting/staying and betting. They also have a bankroll/wallet
class player {

  float runningCount = 0;
  int cardsCounted = 0;

  int decks = G_NUM_DECKS;
  int bankroll = G_STARTING_BANKROLL;
  countingMethod hisCountingMethod = HiOpt2;

  c_matrix hisConfusionMatrix;

public:
  player();
  card seeCard(card a);
  void resetCount();
  int getBet();
  action getBasicStrategyAction(gamestate a);
  action getAction(gamestate g, int stackIndex); //for a stack in a gamestate, will he hit, stay, surrender, etc.
  bool takesInsurance();
  float getTrueCount();
  float getBankroll();
  void getMoney(float a);
  void loseMoney(float a);
  void setCountingMethod(countingMethod a);
  void setConfusionMatrix(c_matrix a);
  void resetAll();

  card perceive(card real);
};

#endif
