#ifndef MATRIX
#define MATRIX

#include <array>
#include <string>
#include "common.h"

class c_matrix {

  std::array<std::array<float, 13>, 13> weights;

  int errorRow = -1;
  float badSum = -1;

public:

  c_matrix();

  c_matrix(std::string fileaddress);

  int verify();

  void setPerception(matrixCard real, matrixCard perceived, float chance);

  //interpolate between this matrix and the identity matrix
  //with a == 1, it returns the identity matrix
  void perfectify(float a);
  

  // USAGE
  float getPerception(matrixCard real, matrixCard perceived);
  
  //given a real card, return the perception
  card perceive(card real, Rng& rng);

  
  // TESTING
  void printWeights();

  void printWeight(matrixCard real, matrixCard perceived);
};

#endif
