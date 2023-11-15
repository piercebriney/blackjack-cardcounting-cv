#ifndef MATRIX
#define MATRIX

#include <vector>
#include <string>
#include "common.h"

class c_matrix {
  //52x52 2d vector
  std::vector<std::vector<float>> weights;

  int errorRow = -1;
  float badSum = -1;

public:

  //CREATION
  void allocate();

  c_matrix();

  c_matrix(std::string fileaddress);

  int verify();

  void setPerception(cardName real, cardName perceived, float chance);

  //interpolate between this matrix and the identity matrix
  //with a == 1, it returns the identity matrix
  void perfectify(float a);
  

  // USAGE
  float getPerception(cardName real, cardName perceived);
  
  //given a real card, return the perception
  cardName perceive(cardName real);


  
  // TESTING
  void printWeights();

  void printWeight(cardName real, cardName perceived);

  void matrixTest();

  int perceptionTest(cardName real);

  int comprehensiveTest();

};

#endif
