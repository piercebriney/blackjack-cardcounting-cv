#ifndef PLAYER
#define PLAYER

#include "matrix.h"
#include "common.h"

//this class is meant to view cards, keep a running count, and output info about hitting/staying and betting. They also have a bankroll/wallet
class player {

  float runningCount = 0;
  int cardsCounted = 0;

  int decks = G_NUM_DECKS;
  long bankroll = G_STARTING_BANKROLL;
  long starting_bankroll = G_STARTING_BANKROLL;
  long max_bet = G_MAXIMUM_BET;
  countingMethod hisCountingMethod = HiLo;

  c_matrix hisConfusionMatrix;

public:
  long min_bet = G_MINIMUM_BET;
  player(int _decks, long _starting_bankroll, long _min_bet, long max_bet);
  card seeCard(card a, Rng& rng);
  void resetCount();
  long getBet();
  action getBasicStrategyAction(gamestate& a);
  action getAction(gamestate& g, int stackIndex); //for a stack in a gamestate, will he hit, stay, surrender, etc.
  bool takesInsurance();
  float getTrueCount();
  long getBankroll();
  void getMoney(long a);
  void loseMoney(long a);
  void setCountingMethod(countingMethod a);
  void setConfusionMatrix(c_matrix& a);
  void resetAll();

  card perceive(card real, Rng& rng);
};

#endif
