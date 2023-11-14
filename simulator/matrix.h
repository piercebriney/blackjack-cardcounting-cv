#include <vector>

typedef enum cardName {_2S,_3S,_4S,_5S,_6S,_7S,_8S,_9S,_10S,_JS,_QS,_KS,_AS,   
                       _2H,_3H,_4H,_5H,_6H,_7H,_8H,_9H,_10H,_JH,_QH,_KH,_AH,
                       _2C,_3C,_4C,_5C,_6C,_7C,_8C,_9C,_10C,_JC,_QC,_KC,_AC,
                       _2D,_3D,_4D,_5D,_6D,_7D,_8D,_9D,_10D,_JD,_QD,_KD,_AD};


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
