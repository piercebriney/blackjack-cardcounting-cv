#include "common.h"
#include "shoe.h"
#include <algorithm>
#include <iostream>

using namespace std;

void shoe::printContents() {
  for(card n : contents) {
    cout << getCardName(n) << endl;
  }
}

void shoe::addDeck() {
  for(int i = 0; i < 52; i++) {
    contents.push_back( (card)i);
  }
}

void shoe::shuffle() {
  std::random_shuffle(contents.begin(), contents.end(), RNG());
}

void shoe::reset() {
  this->contents.clear();
  for(int i = 0; i < G_NUM_DECKS; i++) {
    this->addDeck();
  }
  this->shuffle();
}

void shoe::testBalancedCounting() {
  int sum = 0;
  cout << "lets count " << endl;
  for(card a : contents) {
    PRINT(getCardName(a));
    sum += getCountFromCard(a, HiLo);
  }
  cout << "The final count is:" << endl;
  cout << sum << endl;
}

card shoe::drawCard() {
  card ret = contents.back();
  contents.pop_back();
  return ret;
}

int shoe::getCardsLeftInShoe() {
  return contents.size();
}

vector<card> shoe::getShoe(){
  return this->contents;
}