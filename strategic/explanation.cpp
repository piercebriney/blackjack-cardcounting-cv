#include "analysis.h"
#include "common.h"
#include "matrix.h"
#include "player.h"
#include "shoe.h"
#include <iostream>
#include <string>
#include "strategy.h"
#include "dealer.h"
#include <iomanip>
using namespace std;

int main() {
  srand(time(0));
 
  commonInit();
  loadStrategy();

  player joseph;
  joseph.setCountingMethod(HiLo);

  PRINT("Practice table-lookup");


  while(1) {

    card playerCardA, playerCardB, dealerCardA;
    { //Replace this with CV stuff
      playerCardA = getRandomCard();
      playerCardB = getRandomCard();
      dealerCardA = getRandomCard();
    }

    gamestate g;
    //construct the gamestate
    {
      stack playerCards;
      playerCards.push_back(_AS);
      playerCards.push_back(joseph.seeCardP(playerCardA));
      playerCards.push_back(joseph.seeCardP(playerCardB));
   
      g.perceivedStacks.push_back(playerCards);
      g.stacks.push_back(playerCards);
      g.dealersPerceivedCards.push_back(joseph.seeCardP(dealerCardA));
      g.dealersCards.push_back(joseph.seeCardP(dealerCardA));
    }
  
    action doThis;
    { //Get action like this
      doThis = joseph.getAction(g, 0);
    }
    PRINT(getActionName(doThis));

    int betThis;
    { //Get a bet like this
      betThis = joseph.getBet();
    }
    PRINT(betThis);
      

  }

  

  


}
