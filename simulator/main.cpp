#include "common.h"
#include "matrix.h"
#include "player.h"
#include "shoe.h"
#include <iostream>
#include <string>
using namespace std;


void loadMatrixFromFile(string fileaddress) {
  
}

int main() {
  srand(time(0));
  commonInit();

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

  myShoe.printContents();
  myShoe.testBalancedCounting();

}
