#include "common.h"
#include "shoe.h"
#include <algorithm>
#include <iostream>

using namespace std;

void shoe::printContents() {
  for(cardName n : contents) {
    cout << getCardName(n) << endl;
  }
}

void shoe::addDeck() {
  for(int i = 0; i < 52; i++) {
    contents.push_back( (cardName)i);
  }
}

void shoe::shuffle() {
  std::random_shuffle(contents.begin(), contents.end());
}

void shoe::reset() {
  for(int i = 0; i < G_NUM_DECKS; i++) {
    this->addDeck();
  }
  this->shuffle();
}

void shoe::testBalancedCounting() {
  int sum = 0;
  cout << "lets count " << endl;
  for(cardName a : contents) {
    PRINT(getCardName(a));
    sum += getCountFromCard(a, HiOpt2);
  }
  cout << "The final count is:" << endl;
  cout << sum << endl;
}

cardName shoe::drawCard() {
  cardName ret = contents.back();
  contents.pop_back();
  return ret;
}

int shoe::getCardsLeftInShoe() {
  return contents.size();
}
