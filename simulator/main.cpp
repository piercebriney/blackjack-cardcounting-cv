#include "common.h"
#include "matrix.h"
#include "player.h"
#include "shoe.h"
#include <iostream>
#include <string>
#include "strategy.h"
#include "dealer.h"
#include <iomanip>
using namespace std;


void loadMatrixFromFile(string fileaddress) {
  
}

int main() {
  //srand(time(0));
  
  cout << "Input seed:";
  string seedSTR;
  getline(cin, seedSTR);
  if(seedSTR.size() == 0) {
    int seed = time(0);
    cout << "Using seed " << seed << "." << endl;
    srand(time(0));
  } else {
    cout << "Using seed " << seedSTR << "." << endl;
    srand(stoi(seedSTR));
  }

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
  joseph.setConfusionMatrix(myMatrix);
  
  //Creates shoe, fills shoe with cards, and shuffles deck
  shoe myShoe;
  myShoe.reset();

  //Creates dealer object and set the shoe to the one made previously
  dealer myDealer;
  myDealer.setShoe(myShoe);

  //!!! Finish main playing logic
  for(int i = 0; i < G_NUM_ROUNDS; i++){
    cout << endl << "----------" << "Play round " << i << "/" << G_NUM_ROUNDS << " ----------" << endl;
    if(myDealer.playRound(&joseph)) { cout << "Player went bankrupt." << endl; break;}
    cout << "Bankroll: " << std::setprecision(100) << joseph.getBankroll() << endl;
  }

  cout << endl;
  cout << "Player's final bankroll is: " << std::setprecision(100) << joseph.getBankroll() << endl;
  //cout << "Player's final bankroll is: " << joseph.getBankroll() << endl;
}
