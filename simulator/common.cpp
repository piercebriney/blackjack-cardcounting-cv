#include "common.h"
#include <stdexcept>
#include <iostream>

using namespace std;

std::vector<std::vector<int>>g_countingMethods;

void commonInit() {
  //counting-methods are defined in this order:
  // A 2 3 4 5 6 7 8 9 10
  vector<int> HiLo = {-1, 1, 1, 1, 1, 1, 0, 0, 0, -1}; //balanced
  vector<int> HiOpt2 ={0, 1, 1, 2, 2, 1, 1, 0, 0, -2}; //balanced
  g_countingMethods.push_back(HiLo);
  g_countingMethods.push_back(HiOpt2);
}

string getCardName(card a) {
  switch (a) {
    case _2S:
      return "2S";
    case _3S:
      return "3S";
    case _4S:
      return "4S";
    case _5S:
      return "5S";
    case _6S:
      return "6S";
    case _7S:
      return "7S";
    case _8S:
      return "8S";
    case _9S:
      return "9S";
    case _10S:
      return "10S";
    case _JS:
      return "JS";
    case _QS:
      return "QS";
    case _KS:
      return "KS";
    case _AS:
      return "AS";
  
    case _2H:
      return "2H";
    case _3H:
      return "3H";
    case _4H:
      return "4H";
    case _5H:
      return "5H";
    case _6H:
      return "6H";
    case _7H:
      return "7H";
    case _8H:
      return "8H";
    case _9H:
      return "9H";
    case _10H:
      return "10H";
    case _JH:
      return "JH";
    case _QH:
      return "QH";
    case _KH:
      return "KH";
    case _AH:
      return "AH";
  
    case _2C:
      return "2C";
    case _3C:
      return "3C";
    case _4C:
      return "4C";
    case _5C:
      return "5C";
    case _6C:
      return "6C";
    case _7C:
      return "7C";
    case _8C:
      return "8C";
    case _9C:
      return "9C";
    case _10C:
      return "10C";
    case _JC:
      return "JC";
    case _QC:
      return "QC";
    case _KC:
      return "KC";
    case _AC:
      return "AC";
  
    case _2D:
      return "2D";
    case _3D:
      return "3D";
    case _4D:
      return "4D";
    case _5D:
      return "5D";
    case _6D:
      return "6D";
    case _7D:
      return "7D";
    case _8D:
      return "8D";
    case _9D:
      return "9D";
    case _10D:
      return "10D";
    case _JD:
      return "JD";
    case _QD:
      return "QD";
    case _KD:
      return "KD";
    case _AD:
      return "AD";
  }
  return "ER";
}

string getEffectiveCardName(effectiveCard a) {
  switch (a) {
    case _A:
      return "A";
    case _2:
      return "2";
    case _3:
      return "3";
    case _4:
      return "4";
    case _5:
      return "5";
    case _6:
      return "6";
    case _7:
      return "7";
    case _8:
      return "8";
    case _9:
      return "9";
    case _T:
      return "T";
  }
  return "ER";
}

string getEffectiveCardName(card a) {
  return getEffectiveCardName(getEffectiveCard(a));
}

int getEffectiveCardValue(effectiveCard a) {
  switch (a) {
    case _A:
      //careful
      return 1;
    case _2:
      return 2;
    case _3:
      return 3;
    case _4:
      return 4;
    case _5:
      return 5;
    case _6:
      return 6;
    case _7:
      return 7;
    case _8:
      return 8;
    case _9:
      return 9;
    case _T:
      return 10;
  }
  return 0;
}

int getEffectiveCardValue(card a) {
  return getEffectiveCardValue(getEffectiveCard(a));
}

float getCountFromCard(card a, countingMethod b) {
  vector<int>* thisCountingMethod;
  thisCountingMethod = &g_countingMethods[b];
  return thisCountingMethod->at(getEffectiveCard(a));
}

effectiveCard getEffectiveCard(card a) {
  int rank = a % 13;
  effectiveCard ret;
  if(rank < 9) {
    ret = (effectiveCard)(rank+1);
  } else if(rank < 12) {
    ret = _T;
  } else {
    ret = _A;
  }
  return ret;
}

void printPlayerCards(gamestate g, int stackIndex){
  for(card n : g.stacks[stackIndex]){
    cout << getCardName(n) << " ";
  }
  cout << endl;
}

void printStack(stack s) {
  for(card c : s) {
    cout << getCardName(c) << " ";
  }
  cout << endl;
}

bool isBlackjack(card a, card b) {
  return 
    (getEffectiveCard(a) == _T  && getEffectiveCard(b) == _A)
    ||
    (getEffectiveCard(a) == _A  && getEffectiveCard(b) == _T);
}

int getIdealCount(stack s) {
  stack nonAces;
  stack aces;
  for(auto c : s) {
    if(getEffectiveCard(c) == _A) {
      aces.push_back(c);
    } else {
      nonAces.push_back(c);
    }
  }
  int hardTotal = 0;
  for(auto c : nonAces) {
    hardTotal += getEffectiveCardValue(c);
  }

  int softTotal = 0;
  softTotal = aces.size();
  int validLowTotal = 0;
  if(hardTotal + softTotal <= 21) {
    validLowTotal = hardTotal + softTotal;
  } else {
    return 22; //definitely bust
  }

  //try to get the total higher by counting aces as 11 instead of 1
  int validTotal = validLowTotal;
  for(int i = 0; i < aces.size(); i++) {
    int experimentalTotal = validTotal - 1 + 11;
    if(experimentalTotal <= 21) {
      validTotal = experimentalTotal;
    }
  }
  return validTotal;
}

card getRandomCard() {
  return (card)(rand() % 52);
}
