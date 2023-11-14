#include <fstream>
#include <iostream>
#include "matrix.h"

using namespace std;

// print value
#define PRINT(a)                          \
{                                         \
  std::cout << #a << ": " << (a) << endl; \
}

string getName(cardName it) {
  switch (it) {
    case _2S:
      return "2S";
    case _3S:
      return "3S";
    case _4S:
      return "4S";
    case _5S:
      return "5S";
    case _6S:
      return "6S";
    case _7S:
      return "7S";
    case _8S:
      return "8S";
    case _9S:
      return "9S";
    case _10S:
      return "10S";
    case _JS:
      return "JS";
    case _QS:
      return "QS";
    case _KS:
      return "KS";
    case _AS:
      return "AS";
  
    case _2H:
      return "2H";
    case _3H:
      return "3H";
    case _4H:
      return "4H";
    case _5H:
      return "5H";
    case _6H:
      return "6H";
    case _7H:
      return "7H";
    case _8H:
      return "8H";
    case _9H:
      return "9H";
    case _10H:
      return "10H";
    case _JH:
      return "JH";
    case _QH:
      return "QH";
    case _KH:
      return "KH";
    case _AH:
      return "AH";
  
    case _2C:
      return "2C";
    case _3C:
      return "3C";
    case _4C:
      return "4C";
    case _5C:
      return "5C";
    case _6C:
      return "6C";
    case _7C:
      return "7C";
    case _8C:
      return "8C";
    case _9C:
      return "9C";
    case _10C:
      return "10C";
    case _JC:
      return "JC";
    case _QC:
      return "QC";
    case _KC:
      return "KC";
    case _AC:
      return "AC";
  
    case _2D:
      return "2D";
    case _3D:
      return "3D";
    case _4D:
      return "4D";
    case _5D:
      return "5D";
    case _6D:
      return "6D";
    case _7D:
      return "7D";
    case _8D:
      return "8D";
    case _9D:
      return "9D";
    case _10D:
      return "10D";
    case _JD:
      return "JD";
    case _QD:
      return "QD";
    case _KD:
      return "KD";
    case _AD:
      return "AD";
  }
}

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
  string realName = getName(real);
  float totalTrials = 500;
  float totalRight = 0;
  //cout << "Perceptions for " << realName <<  ":" << endl;

  for(int i = 0; i < totalTrials; i++) {
    string perceivedName = getName(perceive(real));
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
