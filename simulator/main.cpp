#include "analysis.h"
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
  
  cout << "Input seed:\n>";
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

  printf("Input name of c_matrix:\n>");
  
  string input;
  cin >> input;

  string fileaddress = "matrix/" + input + ".csv";

  c_matrix myMatrix = c_matrix(fileaddress);

  printf("Input matrix improvement coefficient (0->unchanged, 1->identity matrix):\n>");
  int perfectness;
  cin >> perfectness;

  myMatrix.perfectify(perfectness);

  player joseph;
  joseph.setCountingMethod(HiLo);
  joseph.setConfusionMatrix(myMatrix);
  
  //Creates shoe, fills shoe with cards, and shuffles deck
  shoe myShoe;
  myShoe.reset();

  //Creates dealer object and set the shoe to the one made previously
  dealer myDealer;
  myDealer.setShoe(myShoe);
  
  int lastBankroll = joseph.getBankroll();
  //!!! Finish main playing logic
  for(int i = 0; i < G_NUM_ROUNDS; i++){
    cout << endl << "----------" << "Play round " << (i+1) << "/" << G_NUM_ROUNDS << " ----------" << endl;
    if(myDealer.playRound(&joseph)) { cout << "Player went bankrupt." << endl; break;}
    cout << "Bankroll: " << std::setprecision(100) << joseph.getBankroll() << endl;
    int change = joseph.getBankroll() - lastBankroll;
    cout << "Change in bankroll: " << std::setprecision(100) << change << endl;
    lastBankroll = joseph.getBankroll();
  }

  cout << endl;
  cout << "Player's final bankroll is: " << std::setprecision(100) << joseph.getBankroll() << endl;
  int profit = joseph.getBankroll() - G_STARTING_BANKROLL;
  int hourlyWage = profit / (G_NUM_ROUNDS / G_NUM_ROUNDS_PER_HOUR);
  cout << "That works out to about " << hourlyWage << " $ per hour." << endl;
  
 analysis a(myMatrix);
 double x = a.getAverageProfit(100, G_NUM_ROUNDS);
 

}
