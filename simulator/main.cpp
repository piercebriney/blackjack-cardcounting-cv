#include "common.h"
#include "matrix.h"
#include "player.h"
#include "shoe.h"
#include <iostream>
#include <string>
#include "strategy.h"
using namespace std;


void loadMatrixFromFile(string fileaddress) {
  
}

int main() {
  srand(time(0));
  commonInit();
  loadStrategy();

  printf("What matrix will be loaded?\n>");
  
  string input;
  cin >> input;

  string fileaddress = "matrix/" + input + ".csv";

  c_matrix myMatrix = c_matrix(fileaddress);
  
  myMatrix.perfectify(1);

  player joseph;

  joseph.setCountingMethod(HiOpt2);
  
  shoe myShoe;

  myShoe.reset();

  //PRINT(lookup(g_hardTotalsTable, "2", "14"));

  gamestate imaginaryGamestate;
  imaginaryGamestate.playersCards.push_back(_4S);
  imaginaryGamestate.playersCards.push_back(_9S);
  imaginaryGamestate.dealersCards.push_back(_AH);

  action whatToDo = getHardTotalsAction(imaginaryGamestate);
  if(whatToDo == hit) {
    printf("Action is hit");
  } else {
    printf("Action is stay");
  }

}
