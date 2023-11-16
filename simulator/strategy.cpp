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

string lookup(table table, string column, string row) {
  
  int rowIndex = -1; int columnIndex = -1;
  for(int i = 0; i < table.size(); i++) {
    if(table[i][0] == row) {
      rowIndex = i;
      break;
    }
  }
  if(rowIndex == -1) {
    cout << "Lookup-error with nonexistant row " << row << " and column " << column << endl;
    return "error";
  }

  for(int i = 0; i < table[0].size(); i++) {
    if(table[0][i] == column) {
      columnIndex = i;
      break;
    }
  }
  if(columnIndex == -1) {
    cout << "Lookup-error with row " << row << " and nonexistant column " << column << endl;
    return "error";
  }

  return table[rowIndex][columnIndex];
}

action getHardTotalsAction(gamestate g) {

  int playerSum = 0;
  for(auto n : g.playersCards) {
    playerSum+=getEffectiveCardValue(getEffectiveCard(n));
  }

  cardName dealerCard;
  dealerCard = g.dealersCards[0];

  string column, row;
  row = to_string(playerSum);
  column = getEffectiveCardName(getEffectiveCard(dealerCard));
  //!!! add functionality for doubling down
  string result = lookup(g_hardTotalsTable, column, row);
  if(result == "S") {
    return stay;
  } else {
    return hit;
  }
}

action getSoftTotalsAction(gamestate g) {
  cardName nonAce = _AD;
  if(g.playersCards.size() > 2) {
    throw std::invalid_argument("getSoftTotalsAction() called but player has more than two cards.");
  }
  if(getEffectiveCardName(getEffectiveCard(g.playersCards[0])) == "A") {
    nonAce = g.playersCards[1];
  } else if(getEffectiveCardName(getEffectiveCard(g.playersCards[1])) == "A") {
    nonAce = g.playersCards[0];
  }
  if(nonAce == _AD) {
    throw std::invalid_argument("getSoftTotalsAction() called but player doesn't have an ace.");
  }

  string column = getEffectiveCardName(getEffectiveCard(nonAce));
  string row = getEffectiveCardName(getEffectiveCard(g.dealersCards[0]));
  string result = lookup(g_softTotalsTable, column, row);

  //!!! add functionality for doubling down
  if(result == "S" || "Ds") {
    return stay;
  } else {
    return hit;
  }
}

bool shouldPlayerSplit(gamestate g) {
  if(g.playersCards[0] != g.playersCards[1]) {
    throw std::invalid_argument("getSplitAction() called but player's cards aren't the same.");
  }
  string column = getEffectiveCardName(getEffectiveCard(g.playersCards[0]));
  string row = getEffectiveCardName(getEffectiveCard(g.dealersCards[0]));
  string result = lookup(g_pairSplittingTable, column, row);
  if(result == "Y") {
    return 1;
  } else {
    return 0;
  }
}

bool shouldPlayerSurrender(gamestate g) {
  int playerSum = 0;
  for(auto n : g.playersCards) {
    playerSum+=getEffectiveCardValue(getEffectiveCard(n));
  }

  cardName dealerCard;
  dealerCard = g.dealersCards[0];
  
}

void loadStrategy() {

  //Cells in the first row represent the dealer's faceup card
  //Cells in the first column represent the player's total
  //The other cells indicate S for stay, H for hit, and D for double if possible, and otherwise hit
  g_hardTotalsTable = 
  {
    {""  , "2", "3", "4", "5", "6", "7", "8", "9", "10", "A"},
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

  //Cells in the first row represent the dealer's faceup card
  //Cells in the first column represent the player's card which is not an ace
  //the other cells indicate S for stay, H for hit, Ds for double or stay
  //and D for double or hit
  g_softTotalsTable = 
  {
    {""  , "2", "3" , "4" , "5" , "6" , "7" , "8" , "9" , "10", "A" },
    {"9", "S" , "S" , "S" , "S" , "S" , "S" , "S" , "S" , "S" , "S" },
    {"8", "S" , "S" , "S" , "S" , "Ds", "S" , "S" , "S" , "S" , "S" },
    {"7", "Ds", "Ds", "Ds", "Ds", "Ds", "S" , "S" , "H" , "H" , "H" },
    {"6", "H" , "D" , "D" , "D" , "D" , "H" , "H" , "H" , "H" , "H" },
    {"5", "H" , "H" , "D" , "D" , "D" , "H" , "H" , "H" , "H" , "H" },
    {"4", "H" , "H" , "D" , "D" , "D" , "H" , "H" , "H" , "H" , "H" },
    {"3", "H" , "H" , "H" , "D" , "D" , "H" , "H" , "H" , "H" , "H" },
    {"2", "H" , "H" , "H" , "D" , "D" , "H" , "H" , "H" , "H" , "H" },
  };

  //Cells in the first row represent the dealer's faceup card
  //Cells in the first column represent the player's card which is not an ace
  //the other cells indicate Y for "yes, split", N for "no, don't split", and
  //Yn for "yes, split if you can double after"
  g_pairSplittingTable =
  {
    {""  , "2", "3" , "4" , "5" , "6" , "7" , "8" , "9" , "10", "A" },
    {"A" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" },
    {"10", "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" },
    {"9" , "Y" , "Y" , "Y" , "Y" , "Y" , "N" , "Y" , "Y" , "N" , "N" },
    {"8" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" },
    {"7" , "Y" , "Y" , "Y" , "Y" , "Y" , "Y" , "N" , "N" , "N" , "N" },
    {"6" , "Yn", "Y" , "Y" , "Y" , "Y" , "N" , "N" , "N" , "N" , "N" },
    {"5" , "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" },
    {"4" , "N" , "N" , "N" , "Yn", "Yn", "N" , "N" , "N" , "N" , "N" },
    {"3" , "Yn", "Yn", "Y" , "Y" , "Y" , "Y" , "N" , "N" , "N" , "N" },
    {"2" , "Yn", "Yn", "Y" , "Y" , "Y" , "Y" , "N" , "N" , "N" , "N" },
  };

  //Cells in the first row represent the dealer's faceup card
  //Cells in the first column represent the sum of the values of the player's cards
  //the other cells indicate Y for "yes, surrender" or N for "No, don't surrender"
  g_lateSurrenderTable = 
  {
    {""  , "2", "3" , "4" , "5" , "6" , "7" , "8" , "9" , "10", "A" },
    {"16", "N", "N" , "N" , "N" , "N" , "N" , "N" , "Y" , "Y" , "Y" },
    {"15", "N", "N" , "N" , "N" , "N" , "N" , "N" , "N" , "Y" , "N" },
    {"14", "N", "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" , "N" },
  };

}
