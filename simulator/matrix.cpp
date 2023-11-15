#include <fstream>
#include <iostream>
#include "matrix.h"

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


void c_matrix::setPerception(cardName real, cardName perceived, float chance) {
  weights[real][perceived] = chance;
}

float c_matrix::getPerception(cardName real, cardName perceived) {
  return weights[real][perceived];
}

void c_matrix::allocate() {
  for(int i = 0; i < 52; i++) {
    weights.push_back({});
    for(int j = 0; j < 52; j++) {
      weights[weights.size() - 1].push_back({});
    }
  }
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
  this->allocate();
}

c_matrix::c_matrix(string fileaddress) {
  this->allocate();

  cout << "Loading matrix " << fileaddress << " ..." << endl;

  ifstream input(fileaddress);
  string line;
  getline(input,line); //jump over first line
  int i = 0;
  for(string line; getline(input, line);) {
    //for each line in input...
    vector<string> x = splitString(line, ',');
    for(int j = 0; j < 52; j++) {
      setPerception((cardName)i, (cardName)j, stof(x[j+1]));
    }
    i++;
  }

  if(this->verify() != 0) {
    cout << "Error: sum of odds in row " << errorRow << " equals " << badSum << endl;
  }
  cout << "Confusion matrix loaded successfully" << endl;
}

cardName c_matrix::perceive(cardName real) {
  float randomness = ( (double) rand() / RAND_MAX );
  float sumOfOdds = 0;
  //cout << "Randomness is " << randomness << endl;

  for(int i = 0; i < 52; i++) {
    if(sumOfOdds >= randomness) {
      return (cardName)(i-1);
    }
    sumOfOdds += getPerception(real, (cardName)(i));
  }
  return (cardName)51;
}


void c_matrix::printWeights() {
  for(auto x : weights) {
    for(auto y : x) {
      cout << y << " ";
    }
    cout << endl;
  }
}

void c_matrix::printWeight(cardName real, cardName perceived) {
  cout << weights[real][perceived] << endl;
}

void c_matrix::matrixTest() {
  cout << "Hmm, what is the chance that a 3D is perceived as a 2S?" << endl;
  printWeight(_3D, _2S);

  cout << "Hmm, what is the chance that an AD is perceived as an AD?" << endl;
  printWeight(_AD, _AD);

  cout << "Hmm, what is the chance that an KH is perceived as an 9S?" << endl;
  printWeight(_KH, _9S);
}

int c_matrix::perceptionTest(cardName real) {
  string realName = getCardName(real);
  float totalTrials = 500;
  float totalRight = 0;
  //cout << "Perceptions for " << realName <<  ":" << endl;

  for(int i = 0; i < totalTrials; i++) {
    string perceivedName = getCardName(perceive(real));
    //cout << perceivedName << endl;
    if(perceivedName == realName) {
      totalRight ++;
    }
  }

  float experimentalSuccess = totalRight / totalTrials;
  float theoreticalSuccess = getPerception(real, real);
  //PRINT(experimentalSuccess);
  //PRINT(theoreticalSuccess);
  const float tolerance = 0.1;

  if(abs(experimentalSuccess - theoreticalSuccess) > tolerance) {
    return 1;
  }
  return 0;
}

int c_matrix::comprehensiveTest() {
  int res = 0;
  for(int card = 0; card < 52; card++) {
    if(perceptionTest((cardName)card) == 1) {
      res = 1;
      break;
    }
  }
  if(res) {
    cout << "Comprehensive test failed." << endl;
  } else {
    cout << "Comprehensive test succeeded." << endl;
  }
  return res;
}

void c_matrix::perfectify(float a) {
  if(a < 0 || a > 1) {return;}
  for(int i = 0; i < 52; i++) {
    for(int j = 0; j < 52; j++) {
      float idealValue = 0;
      if(i == j) { idealValue = 1;}
      weights[i][j] = (weights[i][j] * (1-a)) + idealValue * a;
    }
  }
}
