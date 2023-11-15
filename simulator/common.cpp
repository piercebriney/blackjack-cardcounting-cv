#include "common.h"

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

string getCardName(cardName a) {
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

float getCountFromCard(cardName a, countingMethod b) {
  vector<int>* thisCountingMethod;
  thisCountingMethod = &g_countingMethods[b];
  return thisCountingMethod->at(getEffectiveCard(a));
}

effectiveCard getEffectiveCard(cardName a) {
  int rank = a % 13;
  effectiveCard ret;
  if(rank < 9) {
    ret = (effectiveCard)(rank+1);
  } else if(rank < 12) {
    ret = _10;
  } else {
    ret = _A;
  }
  return ret;
}

