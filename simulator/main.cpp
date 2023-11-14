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
 
  //myMatrix.perceptionTest(_AD);

  myMatrix.comprehensiveTest();
}
