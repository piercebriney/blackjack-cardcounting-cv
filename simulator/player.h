#ifndef PLAYER
#define PLAYER

#include "matrix.h"

//this class is meant to view cards, keep a running count, and output info about hitting/staying and betting. They also have a bankroll/wallet
class player {
  float runningCount = 0;
  float bankroll = 0;
  enum strategy { hilo };
  strategy hisStrategy = hilo;

public:
  
  void seeCard();
  void getBet();
  void getAction(); //hitting/staying

};

#endif
