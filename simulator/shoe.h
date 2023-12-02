#ifndef SHOE
#define SHOE
#include "common.h"
#include <vector>

//a class meant to represet a shoe, or a machine containing multiple decks of cards

struct RNG {
  int operator() (int n) {
        return std::rand() / (1.0 + RAND_MAX) * n;
    }
};
class shoe {
  std::vector<card> contents;

public:
  void reset(); //reset the shoe based on G_NUM_DECKS
  void shuffle();
  void addDeck();
  int getCardsLeftInShoe();
  card drawCard();
  void printContents();
  void testBalancedCounting(); //balanced countingmethods will return 0 after counting a full deck
  std::vector<card> getShoe();
};

#endif
