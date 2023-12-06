#include "common.h"
#include "strategy.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>


using namespace std;

table g_hardTotalsTable;
table g_softTotalsTable;
table g_pairSplittingTable;
table g_lateSurrenderTable;

mp g_hardTotalsMap;
mp g_softTotalsMap;
mp g_pairSplittingMap;
mp g_lateSurrenderMap;

vector<deviation> g_illustrious_18;

string lookup(mp& m, string& column, string& row) {
    auto it = m.find(row+column);
    if (it != m.end()) {
        return it->second;
    } else {
        return "error";
    }
}

action hardTotals(int playerSum, int dealerCard){
  if(playerSum <= 21 && playerSum >= 17) {
    return stay;
  }else if(playerSum <= 16 && playerSum >= 13){
    if(dealerCard >= 2 && dealerCard <= 6){
      return stay;
    }else{
      return hit;
    }
  }else if(playerSum == 12){
    if(dealerCard >= 4 && dealerCard <= 6){
      return stay;
    }else{
      return hit;
    }
  }else if(playerSum == 11){
    return doubledown;
  }else if(playerSum == 10){
    if(dealerCard >= 2 && dealerCard <= 9){
      return doubledown;
    }else{
      return hit;
    }
  }else if(playerSum == 9){
    if(dealerCard >= 3 && dealerCard <= 6){
      return doubledown;
    }else{
      return hit;
    }
  }else{
    return hit;
  }
}



action getHardTotalsAction(gamestate& g, int stackIndex, float trueCount) {
  int playerSum = 0;
  for(auto c : g.perceivedStacks[stackIndex]) {
    playerSum+=getEffectiveCardValue(getEffectiveCard(c));
  }

  if(playerSum < 8) {
    return hit;
  }

  card dealerCard;
  dealerCard = g.dealersPerceivedCards[0];

  return hardTotals(playerSum, getEffectiveCardValue(getEffectiveCard(dealerCard)));
  //string column, row;
  
  //row = to_string(playerSum);
  //column = getEffectiveCardName(getEffectiveCard(dealerCard));
  //!!! add functionality for doubling down
  // string result = lookup(g_hardTotalsMap, column, row);
  // if(result == "S") {
  //   return stay;
  // } else if (result == "H") {
  //   return hit;
  // } else if (result == "D") {
  //   if(trueCount > 1 || !G_ONLY_DD_WITH_ADV) {
  //     return doubledown;
  //   } else {
  //     return hit;
  //   }
  // }
  return stay;
}

action softTotals(int playerCard, int dealerCard){
  if(playerCard == 9){
    return stay;
  }else if(playerCard == 8){
    if(dealerCard == 6){
      return doubledown;
    }else{
      return stay;
    }
  }else if(playerCard == 7){
    if(dealerCard >= 2 && dealerCard <= 6){
      return doubledown;
    }else if(dealerCard >= 8 && dealerCard <= 8){
      return stay;
    }else{
      return hit;
    }
  }else if(playerCard == 6){
    if(dealerCard >= 3 && dealerCard <= 6){
      return doubledown;
    }else{
      return hit;
    }
  }else if(playerCard == 5 || playerCard == 4){
    if(dealerCard >= 4 && dealerCard <= 6){
      return doubledown;
    }else{
      return hit;
    }
  }else if(playerCard == 3 || playerCard == 2){
    if(dealerCard >= 5 && dealerCard <= 6){
      return doubledown;
    }else{
      return hit;
    }
  }
}

action getSoftTotalsAction(gamestate& g, int stackIndex, float trueCount) {
  //if the player just has an ace, must hit
  if(g.perceivedStacks[stackIndex].size() == 1) {
    if(getEffectiveCard(g.perceivedStacks[stackIndex][0]) == _A) {
      //cout << "" << endl;
      return hit;
    } else {
      return hit; //this is only possible with flawed perception
    }
  }

  int nonAceTotal = 0;
  //get total of all cards which aren't aces
  for(int i = 0; i < g.perceivedStacks[stackIndex].size(); i++) {
    if(getEffectiveCard(g.perceivedStacks[stackIndex][i]) != _A) {
      nonAceTotal += getEffectiveCardValue(g.perceivedStacks[stackIndex][i]);
    }
  }

  if(nonAceTotal == 0) {
    return hit;
  }

  if(nonAceTotal > 9) {
    return stay;
  }

  int playerCardVal = nonAceTotal;
  int dealerCardVal = getEffectiveCardValue(getEffectiveCard(g.dealersPerceivedCards[0]));
  return softTotals(playerCardVal, dealerCardVal);

  // string column = getEffectiveCardName(getEffectiveCard(g.dealersPerceivedCards[0]));
  // string row = to_string(nonAceTotal);
  // string result = lookup(g_softTotalsMap, column, row);

  // //!!! add functionality for doubling down
  // if(result == "S") {
  //   return stay;
  // } else if(result == "H") {
  //   return hit;
  // } else if(result == "Ds") {
  //   if(trueCount > 1 || !G_ONLY_DD_WITH_ADV) {
  //     return doubledown;
  //   } else {
  //     return stay;
  //   }
  // } else if(result == "D") {
  //   if(trueCount > 1 || !G_ONLY_DD_WITH_ADV) {
  //     return doubledown;
  //   } else {
  //     return hit;
  //   }
  // }
  // return stay;
}

bool shouldSplit(int playerCard, int dealerCard){
  if(playerCard == 1){
    return true;
  }else if(playerCard == 10){
    return false;
  }else if(playerCard == 9){
    if(dealerCard == 7 || dealerCard == 10 || dealerCard == 1){
      return false;
    }else {
      return false;
    }
  }else if(playerCard == 8){
    return true;
  }else if(playerCard == 7){
    if(dealerCard >= 2 && dealerCard <= 7){
      return true;
    }else{
      return false;
    }
  }else if(playerCard == 6){
    if(dealerCard >= 2 && dealerCard <= 6){
      return true;
    }else{
      return false;
    }
  }else if(playerCard == 5){
    return false;
  }else if(playerCard == 4){
    if(dealerCard == 5 || dealerCard == 6){
      return true;
    }else{
      return false;
    }
  }else if(playerCard == 2 || playerCard == 3){
    if(dealerCard >= 2 && dealerCard <= 7){
      return true;
    }else{
      return false;
    }
  }
}

bool shouldPlayerSplit(gamestate& g, int stackIndex) {
  if(g.stacks[stackIndex].size() != 2) {
    throw std::invalid_argument("getSplitAction() called but the stack doesn't have exactly two cards.");
  }

  //you can split a king and a ten, or a queen and a king
  if(getEffectiveCardValue(g.stacks[stackIndex][0]) != getEffectiveCardValue(g.stacks[stackIndex][1])) {
    throw std::invalid_argument("getSplitAction() called but the stack's cards aren't identical");
  }

  int dealerCard = getEffectiveCardValue(getEffectiveCard(g.dealersPerceivedCards[0]));
  int playerCard = getEffectiveCardValue(getEffectiveCard(g.perceivedStacks[stackIndex][0]));
  return shouldSplit(playerCard, dealerCard);
  //string column = getEffectiveCardName(getEffectiveCard(g.dealersPerceivedCards[0]));
  //string row = getEffectiveCardName(getEffectiveCard(g.perceivedStacks[stackIndex][0]));
  // string result = lookup(g_pairSplittingMap, column, row);
  // if(result == "Y" || result == "Yn") {
  //   return 1;
  // } else {
  //   return 0;
  // }
}

bool shouldUseHardTotals(gamestate& g, int stackIndex){
  bool shouldHardTotal = true;
  for(int i = 0; i < g.perceivedStacks[stackIndex].size(); i++) {
    if(getEffectiveCard(g.perceivedStacks[stackIndex][i]) == _A) {
      shouldHardTotal = false;
    }
  }
  return shouldHardTotal;
}

bool shouldPlayerSurrender(gamestate& g, int stackIndex) {
  int playerSum = 0;
  for(auto n : g.stacks[stackIndex]) {
    playerSum+=getEffectiveCardValue(getEffectiveCard(n));
  }

  card dealerCard;
  dealerCard = g.dealersCards[0];
  int dealerCardVal = getEffectiveCardValue(getEffectiveCard(dealerCard));
  if(playerSum == 16 && (dealerCardVal == 9 || dealerCardVal == 10 || dealerCardVal == 1)) {
    return true;
  }else{
    return false;
  }

  // string row = to_string(playerSum);
  // string column = getEffectiveCardName(getEffectiveCard(dealerCard));

  // string result = lookup(g_lateSurrenderMap, column, row);
  // if(result == "Y") {return true;} else {return false;}
}

bool shouldPlayerInsure(float trueCount) {
  return (trueCount > 3);
}

action getActionFromDeviations(gamestate& g, int stackIndex, float trueCount) {
  
  for(deviation& d : g_illustrious_18) {
    if(  
        (!d.activeLessThan && trueCount > d.index)
        ||
        (d.activeLessThan && trueCount < d.index)
        ) {

      if(d.useHandNotTotal) {
        if(d.dealerUpCard == getEffectiveCard(g.dealersPerceivedCards[0]) && areStacksEffectivelyEqual(g.perceivedStacks[stackIndex], d.hand)) {
          if(d.result == split && g.stacks.size() >= 4){
            return stay;
          }
          return d.result;
        }
  
      } else {
        int sum = getLowSumOfStack(g.perceivedStacks[stackIndex]);
        if(sum == d.total && d.dealerUpCard == getEffectiveCard(g.dealersCards[0])) { 
          if(d.result == split && g.stacks.size() >= 4){
            return stay;
          }
          return d.result;

        }
      }
    }
  }
  return voidaction;
}





void loadStrategy() {

  //Cells in the first row represent the dealer's faceup card
  //Cells in the first column represent the player's total
  //The other cells indicate S for stay, H for hit, and D for double if possible, and otherwise hit
  g_hardTotalsTable = 
  {
    {""  , "2", "3", "4", "5", "6", "7", "8", "9", "T", "A"},
    {"21", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"20", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"19", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"18", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"17", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
    {"16", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"15", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"14", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"13", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"12", "H", "H", "S", "S", "S", "H", "H", "H", "H", "H"},
    {"11", "D", "D", "D", "D", "D", "D", "D", "D", "D", "D"},
    {"10", "D", "D", "D", "D", "D", "D", "D", "D", "H", "H"},
    {"9" , "H", "D", "D", "D", "D", "H", "H", "H", "H", "H"},
    {"8" , "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
  };
  for(int row = 1; row <= 14; row++){
    for(int col = 1; col <= 10; col++){
      string r = g_hardTotalsTable[row][0];
      string c = g_hardTotalsTable[0][col]; 
      g_hardTotalsMap[r+c] = g_hardTotalsTable[row][col];
    }
  }
  //Cells in the first row represent the dealer's faceup card
  //Cells in the first column represent the player's card which is not an ace
  //the other cells indicate S for stay, H for hit, Ds for double or stay
  //and D for double or hit
  g_softTotalsTable = 
  {
    {"" , "2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "T" , "A" },
    {"9", "S" , "S" , "S" , "S" , "S" , "S" , "S" , "S" , "S" , "S" },
    {"8", "S" , "S" , "S" , "S" , "Ds", "S" , "S" , "S" , "S" , "S" },
    {"7", "Ds", "Ds", "Ds", "Ds", "Ds", "S" , "S" , "H" , "H" , "H" },
    {"6", "H" , "D" , "D" , "D" , "D" , "H" , "H" , "H" , "H" , "H" },
    {"5", "H" , "H" , "D" , "D" , "D" , "H" , "H" , "H" , "H" , "H" },
    {"4", "H" , "H" , "D" , "D" , "D" , "H" , "H" , "H" , "H" , "H" },
    {"3", "H" , "H" , "H" , "D" , "D" , "H" , "H" , "H" , "H" , "H" },
    {"2", "H" , "H" , "H" , "D" , "D" , "H" , "H" , "H" , "H" , "H" },
  };
  


  for(int row = 1; row <= 8; row++){
    for(int col = 1; col <= 10; col++){
      string r = g_softTotalsTable[row][0];
      string c = g_softTotalsTable[0][col]; 
      g_softTotalsMap[r+c] = g_softTotalsTable[row][col];
    }
  }

  //Cells in the first row represent the dealer's faceup card
  //Cells in the first column represent the player's card which is not an ace
  //the other cells indicate Y for "yes, split", N for "no, don't split", and
  //Yn for "yes, split if you can double after"
  g_pairSplittingTable =
  {
    {""  , "2", "3" , "4" , "5" , "6" , "7" , "8" , "9" , "T", "A" },
    {"A" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" },
    {"T", "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" },
    {"9" , "Y" , "Y" , "Y" , "Y" , "Y" , "N" , "Y" , "Y" , "N" , "N" },
    {"8" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" },
    {"7" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "N" , "N" , "N" , "N" },
    {"6" , "Yn", "Y" , "Y" , "Y" , "Y" , "N" , "N" , "N" , "N" , "N" },
    {"5" , "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" },
    {"4" , "N" , "N" , "N" , "Yn", "Yn", "N" , "N" , "N" , "N" , "N" },
    {"3" , "Yn", "Yn", "Y" , "Y" , "Y" , "Y" , "N" , "N" , "N" , "N" },
    {"2" , "Yn", "Yn", "Y" , "Y" , "Y" , "Y" , "N" , "N" , "N" , "N" },
  };
  for(int row = 1; row <= 10; row++){
    for(int col = 1; col <= 10; col++){
      string r = g_pairSplittingTable[row][0];
      string c = g_pairSplittingTable[0][col]; 
      g_pairSplittingMap[r+c] = g_pairSplittingTable[row][col];
    }
  }

  //Cells in the first row represent the dealer's faceup card
  //Cells in the first column represent the sum of the values of the player's cards
  //the other cells indicate Y for "yes, surrender" or N for "No, don't surrender"
  g_lateSurrenderTable = 
  {
    {""  , "2", "3" , "4" , "5" , "6" , "7" , "8" , "9" , "T", "A" },
    {"16", "N", "N" , "N" , "N" , "N" , "N" , "N" , "Y" , "Y" , "Y" },
    {"15", "N", "N" , "N" , "N" , "N" , "N" , "N" , "N" , "Y" , "N" },
    {"14", "N", "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" },
  };
  for(int row = 1; row <= 3; row++){
    for(int col = 1; col <= 10; col++){
      string r = g_lateSurrenderTable[row][0];
      string c = g_lateSurrenderTable[0][col]; 
      g_lateSurrenderMap[r+c] = g_lateSurrenderTable[row][col];
    }
  }
  
  { 

    g_illustrious_18.clear();
    //Donald Schlesinger's illustrious 18
    deviation two;
    two.total = 16;
    two.dealerUpCard = _T;
    two.index = 3;
    two.result = stay;
    g_illustrious_18.push_back(two);
  
    deviation three;
    three.total = 15;
    three.dealerUpCard = _T;
    three.index = 4;
    three.result = stay;
    g_illustrious_18.push_back(three);

    deviation four;
    four.useHandNotTotal = 1;
    four.hand.push_back(_10D);
    four.hand.push_back(_10D);
    four.dealerUpCard = _5;
    four.index = 5;
    four.result = split;
    g_illustrious_18.push_back(four);

    deviation five;
    five.useHandNotTotal = 1;
    five.hand.push_back(_10D);
    five.hand.push_back(_10D);
    five.dealerUpCard = _6;
    five.index = 4;
    five.result = split;
    g_illustrious_18.push_back(five);

    deviation six;
    six.total = 10;
    six.dealerUpCard = _T;
    six.index = 4;
    six.result = doubledown;
    g_illustrious_18.push_back(six);

    deviation seven;
    seven.total = 12;
    seven.dealerUpCard = _3;
    seven.index = 2;
    seven.result = stay;
    g_illustrious_18.push_back(seven);

    deviation eight;
    eight.total = 12;
    eight.dealerUpCard = _2;
    eight.index = 3;
    eight.result = stay;
    g_illustrious_18.push_back(eight);

    deviation nine;
    nine.total = 11;
    nine.dealerUpCard = _A;
    nine.index = 1;
    nine.result = doubledown;
    g_illustrious_18.push_back(nine);

    deviation ten;
    ten.total = 9;
    ten.dealerUpCard = _2;
    ten.index = 1;
    ten.result = doubledown;
    g_illustrious_18.push_back(ten);

    deviation eleven;
    eleven.total = 10;
    eleven.dealerUpCard = _A;
    eleven.index = 4;
    eleven.result = doubledown;
    g_illustrious_18.push_back(eleven);

    deviation twelve;
    twelve.total = 9;
    twelve.dealerUpCard = _7;
    twelve.index = 3;
    twelve.result = doubledown;
    g_illustrious_18.push_back(twelve);

    deviation thirteen;
    thirteen.total = 16;
    thirteen.dealerUpCard = _5;
    thirteen.index = 9;
    thirteen.result = stay;
    g_illustrious_18.push_back(thirteen);

    deviation fourteen;
    fourteen.total = 13;
    fourteen.dealerUpCard = _2;
    fourteen.index = -1;
    fourteen.result = stay;
    g_illustrious_18.push_back(fourteen);

    deviation fifteen;
    fifteen.total = 12;
    fifteen.dealerUpCard = _4;
    fifteen.index = 0;
    fifteen.result = stay;
    fifteen.activeLessThan = 1;
    g_illustrious_18.push_back(fifteen);

    deviation sixteen;
    sixteen.total = 12;
    sixteen.dealerUpCard = _5;
    sixteen.index = -2;
    sixteen.result = stay;
    sixteen.activeLessThan = 1;
    g_illustrious_18.push_back(sixteen);

    deviation seventeen;
    seventeen.total = 12;
    seventeen.dealerUpCard = _6;
    seventeen.index = -1;
    seventeen.result = stay;
    seventeen.activeLessThan = 1;
    g_illustrious_18.push_back(seventeen);

    deviation eighteen;
    eighteen.total = 13;
    eighteen.dealerUpCard = _3;
    eighteen.index = -2;
    eighteen.result = stay;
    eighteen.activeLessThan = 1;
    g_illustrious_18.push_back(eighteen);

  }
  
}
