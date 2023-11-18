#ifndef PLAYER
#define PLAYER

#include "matrix.h"
#include "common.h"

//this class is meant to view cards, keep a running count, and output info about hitting/staying and betting. They also have a bankroll/wallet
class player {

  float runningCount = 0;
  int cardsCounted = 0;

  int decks = G_NUM_DECKS;
  float bankroll = G_STARTING_BANKROLL;
  countingMethod hisCountingMethod = HiOpt2;

public:
  player();
  void seeCard(cardName a);
  void resetCount();
  int getBet();
  action getBasicStrategyAction(gamestate a);
  action getAction(gamestate a); //hitting/staying
  float getBankroll();
  void setCountingMethod(countingMethod a);
  

};

#endif