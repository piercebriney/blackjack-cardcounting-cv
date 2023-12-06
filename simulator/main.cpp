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
#include <random>
using namespace std;


void loadMatrixFromFile(string fileaddress) {
  
}

int main() {
  cout << "Input seed:\n>";
  string seedSTR;
  getline(cin, seedSTR);
  Rng rng{0};
  if(seedSTR.size() == 0) {
    pcg_extras::seed_seq_from<std::random_device> seed_source;
    rng = Rng{seed_source};
    cout << "Using randomized initial state " << rng << endl;
  } else {
    cout << "Using seed " << seedSTR << "." << endl;
    rng = Rng{(__int128 unsigned)stoi(seedSTR)};
  }

  commonInit();
  loadStrategy();

  printf("Input name of c_matrix:\n>");
  
  string input{"mockup.csv"};
  cin >> input;

  string fileaddress = "matrix/" + input + ".csv";

  c_matrix myMatrix = c_matrix(fileaddress);

  printf("Input matrix improvement coefficient (0->unchanged, 1->identity matrix):\n>");
  float perfectness = 1.0;
  cin >> perfectness;

  myMatrix.perfectify(perfectness);

  //player joseph;
  //joseph.setCountingMethod(HiLo);
  //joseph.setConfusionMatrix(myMatrix);
  
  ////Creates shoe, fills shoe with cards, and shuffles deck
  //shoe myShoe;
  //myShoe.reset(rng);

  ////Creates dealer object and set the shoe to the one made previously
  //dealer myDealer;
  //myDealer.setShoe(myShoe);
  
  //int lastBankroll = joseph.getBankroll();
  ////!!! Finish main playing logic
  //gamestate g;
  //for(int i = 0; i < G_NUM_ROUNDS; i++){
  //  //cout << endl << "----------" << "Play round " << (i+1) << "/" << G_NUM_ROUNDS << " ----------" << endl;
  //  if(myDealer.playRound(joseph, g, rng, false)) { cout << "Player went bankrupt." << endl; break;}
  //  //cout << "Bankroll: " << std::setprecision(100) << joseph.getBankroll() << endl;
  //  int change = joseph.getBankroll() - lastBankroll;
  //  //cout << "Change in bankroll: " << std::setprecision(100) << change << endl;
  //  lastBankroll = joseph.getBankroll();
  //}

  //cout << endl;
  //cout << "Player's final bankroll is: " << std::setprecision(100) << joseph.getBankroll() << endl;
  //int profit = joseph.getBankroll() - G_STARTING_BANKROLL;
  //int hourlyWage = profit / (G_NUM_ROUNDS / G_NUM_ROUNDS_PER_HOUR);
  //cout << "That works out to about " << hourlyWage << " $ per hour." << endl;
  
  vector<float> profits;
 analysis a{myMatrix, rng};
 a.runTrials(10000, G_NUM_ROUNDS, rng, profits);
 a.printStats(profits);

 // double x = a.getAverageProfit(10000, G_NUM_ROUNDS, rng);
 //a.testEpsilons(fileaddress);
 //a.getAverageProfit(20, G_NUM_ROUNDS);
 //a.getAverageProfit(100, G_NUM_ROUNDS);
 //a.getAverageProfit(250, G_NUM_ROUNDS);
 //a.getAverageProfit(500, G_NUM_ROUNDS);
 //a.getAverageProfit(750, G_NUM_ROUNDS);
 //a.getAverageProfit(1000, G_NUM_ROUNDS);
 //a.testEpsilons(fileaddress);
}
