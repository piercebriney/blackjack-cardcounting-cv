#include "common.h"
#include "strategy.h"
#include <vector>
#include <string>

using namespace std;

table g_hardTotalsTable;

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

}
