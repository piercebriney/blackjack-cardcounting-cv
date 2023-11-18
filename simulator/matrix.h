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

  void setPerception(card real, card perceived, float chance);

  //interpolate between this matrix and the identity matrix
  //with a == 1, it returns the identity matrix
  void perfectify(float a);
  

  // USAGE
  float getPerception(card real, card perceived);
  
  //given a real card, return the perception
  card perceive(card real);


  
  // TESTING
  void printWeights();

  void printWeight(card real, card perceived);

  void matrixTest();

  int perceptionTest(card real);

  int comprehensiveTest();

};

#endif
