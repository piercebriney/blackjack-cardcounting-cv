#ifndef SHOE
#define SHOE
#include "common.h"
#include <vector>

//a class meant to represet a shoe, or a machine containing multiple decks of cards

class shoe {
  std::vector<card> contents;
  int decks = G_NUM_DECKS;

public:
  shoe(int _decks);
  void reset(Rng& rng); //reset the shoe based on G_NUM_DECKS
  void shuffle(Rng& rng);
  void addDeck();
  int getCardsLeftInShoe();
  card drawCard();
  void printContents();
  void testBalancedCounting(); //balanced countingmethods will return 0 after counting a full deck
  std::vector<card> getShoe();
};

#endif
