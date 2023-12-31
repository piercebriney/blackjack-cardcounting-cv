#include <fstream>
#include <iostream>
#include "matrix.h"
#include <random>

using namespace std;

vector<string> splitString(string s, char delimiter)
{
  int start;
  long long unsigned int end;
  start = 0;
  string token;
  vector<string> ret;
  end = s.find(delimiter, start);
  while (end != string::npos)
  {
    token = s.substr(start, end - start);
    start = end + 1;
    ret.push_back(token);
    end = s.find(delimiter, start);
  }

  ret.push_back(s.substr(start));
  return ret;
}


void c_matrix::setPerception(matrixCard real, matrixCard perceived, float chance) {
  weights[real][perceived] = chance;
}

float c_matrix::getPerception(matrixCard real, matrixCard perceived) {
  return weights[real][perceived];
}

int c_matrix::verify() {
  const float tolerance = 0.0001;
  //make sure that each row adds up to roughly one
  int i = 0;
  for(auto row : weights) {
    
    float sum = 0;
    for(auto entry : row) {
      sum += entry;
    }
    if(abs(1 - sum) > tolerance) {
      cout << "bad abs: " << abs(1-sum) << endl;
      cout << "tolerance: " << tolerance << endl;
      badSum = sum;
      errorRow = i;
      return 1;
    }
    i++;
  }
  return 0;
}

c_matrix::c_matrix() {
}

c_matrix::c_matrix(string fileaddress) {
  ifstream input(fileaddress);
  if (input.fail()) {
      string msg{"unable to load confusion matrix: "};
      msg += fileaddress;
      throw std::runtime_error{msg};
  }
  string line;
  int i = 0;
  for(string line; getline(input, line);) {
    //for each line in input...
    vector<string> x = splitString(line, ' ');
    if (i > 13) {break;}
    for(int j = 0; j < 13; j++) {
      setPerception((matrixCard)j, (matrixCard)i, stof(x[j]));
    }
    i++;
  }

  //normalize
  for(int i = 0; i < 13; i++) {
    float rowSum = 0;
        for(int j = 0; j < 13; j++) {
          rowSum += weights[i][j];
        }
        // don't divide by zero for nan
        if (rowSum > 0.0) {
            for(int j = 0; j < 13; j++) {
              weights[i][j] /= rowSum;
            }
        }
    }
}

card c_matrix::perceive(card real, Rng& rng) {
  std::uniform_real_distribution<float> d(0.0,1.0);
  float randomness = d(rng);
  matrixCard realMatrixCard = getMatrixCard(real);
  float sumOfOdds = 0;
  //cout << "Randomness is " << randomness << endl;

  for(int i = 0; i < 13; i++) {
    if(sumOfOdds >= randomness) {
      return getCard((matrixCard)(i-1));
    }
    sumOfOdds += getPerception(realMatrixCard, (matrixCard)(i));
  }
  return getCard((matrixCard)12);
}


void c_matrix::printWeights() {
  for(auto x : weights) {
    for(auto y : x) {
      cout << y << " ";
    }
    cout << endl;
  }
}

void c_matrix::printWeight(matrixCard real, matrixCard perceived) {
  cout << weights[real][perceived] << endl;
}

void c_matrix::perfectify(float a) {
  if(a < 0 || a > 1) {return;}
  for(int i = 0; i < 13; i++) {
    for(int j = 0; j < 13; j++) {
      float idealValue = 0;
      if(i == j) { idealValue = 1;}
      weights[i][j] = (weights[i][j] * (1-a)) + idealValue * a;
    }
  }
}
