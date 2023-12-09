#include "common.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <array>

using namespace std;

std::vector<std::array<int, 10>>g_countingMethods;

void commonInit() {
  //counting-methods are defined in this order:
  // A 2 3 4 5 6 7 8 9 10
  std::array<int, 10> HiLo = {-1, 1, 1, 1, 1, 1, 0, 0, 0, -1}; //balanced
  std::array<int, 10> HiOpt2 = {0, 1, 1, 2, 2, 1, 1, 0, 0, -2}; //balanced
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

matrixCard getMatrixCard(card a) {
  switch (a) {
    case _2S:
      return M2;
    case _3S:
      return M3;
    case _4S:
      return M4;
    case _5S:
      return M5;
    case _6S:
      return M6;
    case _7S:
      return M7;
    case _8S:
      return M8;
    case _9S:
      return M9;
    case _10S:
      return MJ;
    case _JS:
      return MJ;
    case _QS:
      return MQ;
    case _KS:
      return MK;
    case _AS:
      return MA;


    case _2H:
      return M2;
    case _3H:
      return M3;
    case _4H:
      return M4;
    case _5H:
      return M5;
    case _6H:
      return M6;
    case _7H:
      return M7;
    case _8H:
      return M8;
    case _9H:
      return M9;
    case _10H:
      return MJ;
    case _JH:
      return MJ;
    case _QH:
      return MQ;
    case _KH:
      return MK;
    case _AH:
      return MA;


    case _2C:
      return M2;
    case _3C:
      return M3;
    case _4C:
      return M4;
    case _5C:
      return M5;
    case _6C:
      return M6;
    case _7C:
      return M7;
    case _8C:
      return M8;
    case _9C:
      return M9;
    case _10C:
      return MJ;
    case _JC:
      return MJ;
    case _QC:
      return MQ;
    case _KC:
      return MK;
    case _AC:
      return MA;


    case _2D:
      return M2;
    case _3D:
      return M3;
    case _4D:
      return M4;
    case _5D:
      return M5;
    case _6D:
      return M6;
    case _7D:
      return M7;
    case _8D:
      return M8;
    case _9D:
      return M9;
    case _10D:
      return MJ;
    case _JD:
      return MJ;
    case _QD:
      return MQ;
    case _KD:
      return MK;
    case _AD:
      return MA;

  }
  std::string s{"invalid card in getMatrixCard: "};
  s += std::to_string(a);
  throw std::runtime_error(s);
}

card getCard(matrixCard a) {
  switch (a) {
    case MA:
      return _AS;
    case M2:
      return _2S;
    case M3:
      return _3S;
    case M4:
      return _4S;
    case M5:
      return _5S;
    case M6:
      return _6S;
    case M7:
      return _7S;
    case M8:
      return _8S;
    case M9:
      return _9S;
    case M10:
      return _10S;
    case MJ:
      return _JS;
    case MQ:
      return _QS;
    case MK:
      return _KS;
  }
  std::string s{"invalid card in getCard: "};
  s += std::to_string(a);
  throw std::runtime_error(s);
}

float getCountFromCard(card a, countingMethod b) {
  return g_countingMethods[b].at(getEffectiveCard(a));
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

void printPlayerCards(gamestate& g, int stackIndex){
  for(card n : g.stacks[stackIndex]){
    cout << getCardName(n) << " ";
  }
  cout << endl;
}

void printStack(stack& s) {
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

int getIdealCount(stack& s) {
    int aces = 0;
    int hardTotal = 0;
    for (auto c: s) {
        effectiveCard eff = getEffectiveCard(c);
        if (eff == _A) {
            aces += 1;
        } else {
            hardTotal += getEffectiveCardValue(eff);
        }
    }
    int softTotal = aces;
    int validLowTotal = 0;
    if (hardTotal + softTotal <= 21) {
        validLowTotal = hardTotal + softTotal;
    } else {
        return 22; //definitely bust
    }

    //try to get the total higher by counting aces as 11 instead of 1
    int validTotal = validLowTotal;
    for (int i = 0; i < aces; i++) {
        int experimentalTotal = validTotal - 1 + 11;
        if (experimentalTotal <= 21) {
            validTotal = experimentalTotal;
        }
    }
    return validTotal;
}

bool areStacksEqual(stack& a, stack& b) {
  sort(a.begin(), a.end());
  sort(b.begin(), b.end());
  return a == b;
}

std::array<int, 10> countEffectiveCards(stack& s) {
    std::array<int, 10> arr{};
    for (card c: s) {
        arr[getEffectiveCard(c)] += 1;
    }
    return arr;
}

bool areStacksEffectivelyEqual(stack& a, stack& b) {
    return countEffectiveCards(a) == countEffectiveCards(b);
}

int getLowSumOfStack(stack& a) {
  int sum = 0;
  for(card c : a) {
    sum += getEffectiveCardValue(c);
  }
  return sum;
}
