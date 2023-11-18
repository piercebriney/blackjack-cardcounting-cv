#include "common.h"
#include "matrix.h"
#include "player.h"
#include "shoe.h"
#include <iostream>
#include <string>
#include "strategy.h"
#include "dealer.h"
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

  //Creates Player object and sets counting method to HiOpt2
  player joseph;
  joseph.setCountingMethod(HiOpt2);
  
  //Creates shoe, fills shoe with cards, and shuffles deck
  shoe myShoe;
  myShoe.reset();


  //Creates dealer object and set the shoe to the one made previously
  dealer myDealer;
  myDealer.setShoe(myShoe);
  PRINT(lookup(g_hardTotalsTable, "2", "14"));

  gamestate imaginaryGamestate;
  imaginaryGamestate.stacks[0].push_back(_4S);
  imaginaryGamestate.stacks[0].push_back(_9S);
  imaginaryGamestate.dealersCards.push_back(_AH);

  action whatToDo = getHardTotalsAction(imaginaryGamestate, 0);
  if(whatToDo == hit) {
    printf("Action is hit");
  } else {
    printf("Action is stay");
  }

  //!!! Finish main playing logic
  for(int i = 0; i < G_NUM_ROUNDS; i++){
    cout << "Player's current Bankroll is: " << joseph.getBankroll() << endl;
    myDealer.playRound(joseph);
  }
}
