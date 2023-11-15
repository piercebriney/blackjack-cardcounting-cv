#include <iostream>
#include <string>
#include "matrix.h"
using namespace std;


void loadMatrixFromFile(string fileaddress) {
  
}

int main() {
  srand(time(0));
  printf("What matrix will be loaded?\n>");
  
  string input;
  cin >> input;

  string fileaddress = "matrix/" + input + ".csv";

  c_matrix myMatrix = c_matrix(fileaddress);
  
  myMatrix.perfectify(0.95);

  printf("How many rounds of blackjack to play?");

  PRINT(getEffectiveCard(_2D));
  PRINT(getEffectiveCard(_3D));
  PRINT(getEffectiveCard(_4D));
  PRINT(getEffectiveCard(_5D));
  PRINT(getEffectiveCard(_6D));
  PRINT(getEffectiveCard(_7D));
  PRINT(getEffectiveCard(_8D));
  PRINT(getEffectiveCard(_9D));
  PRINT(getEffectiveCard(_10D));
  PRINT(getEffectiveCard(_JD));
  PRINT(getEffectiveCard(_QD));
  PRINT(getEffectiveCard(_KD));
  PRINT(getEffectiveCard(_AD));
  //getEffectiveCard(_3D);

}
