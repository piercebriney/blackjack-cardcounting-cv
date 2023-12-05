#ifndef DEALER
#define DEALER

#include "common.h"
#include "player.h"
#include <vector>


//a class meant to represent a dealer, who uses a shoe to deal cards to herself and a player, and collects and pays money
class dealer {
  private:
    shoe herShoe;
  public:
    dealer();
    dealer(shoe myShoe);
    int playRound(player* a, Rng& rng);
    void setShoe(shoe myShoe);
};

#endif
