#include "common.h"
#include "shoe.h"
#include <random>
#include <algorithm>
#include <iostream>
#include "pcg/randutils.hpp"

using namespace std;

shoe::shoe(int _decks): decks(_decks) {

}

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

void shoe::shuffle(Rng& rng) {
    // https://www.geeksforgeeks.org/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/
    for (int i = contents.size() - 1; i > 0; i--) {
        int j = rng(i + 1);
        std::swap(contents[i], contents[j]);
    }
}

void shoe::reset(Rng& rng) {
  this->contents.clear();
  for(int i = 0; i < decks; i++) {
    this->addDeck();
  }
  this->shuffle(rng);
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
  if (contents.size() == 0) {
    cout << "EMPTY CONTENTS\n";
    abort();
  }
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
