#ifndef DEALER
#define DEALER

#include "common.h"
#include "player.h"
#include <vector>


//a class meant to represent a dealer, who uses a shoe to deal cards to herself and a player, and collects and pays money
class dealer {
  shoe herShoe;
  int playRound(player a);
};

#endif
