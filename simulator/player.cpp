#include "player.h"

using namespace std;

void player::seeCard(cardName a) {
  cardsCounted++;
  vector<int>* thisCountingMethod;
  thisCountingMethod = &g_countingMethods[hisCountingMethod];
  runningCount += thisCountingMethod->at(getEffectiveCard(a));
}

float player::getBankroll() {
  return bankroll;
}

void player::setCountingMethod(countingMethod a) {
  hisCountingMethod = a;
}

void player::resetCount() {
  runningCount = 0;
  cardsCounted = 0;
}

//if the player doesn't have an advantage, do the minimum bet
int player::getBet() {
  float playerAdvantge;
  float trueCount;

  //this is not quite how humans calculate decks remaining!
  //as such, it may be overestimating playeradvantage
  float decksRemaining = cardsCounted;
  decksRemaining = G_NUM_DECKS - (decksRemaining/52);

  trueCount = runningCount / decksRemaining;

  //playeradvantage is negative if the trueCount is below 1
  if(trueCount <= 1) {
    return G_MINIMUM_BET;
  } else {
    float playerAdvantage = (trueCount - 1)/200; //playeradvantage increases by 0.5% for every trueCount
    //if the player has an advantage, bet the advantage percentage of the bankroll (kelly criterion)
    float ret = playerAdvantage * bankroll;
    if(ret < G_MINIMUM_BET) {ret = G_MINIMUM_BET;}
    if(ret > G_MAXIMUM_BET) {ret = G_MAXIMUM_BET;}
    return ret;
  }
}

//perfect basic strategy reduces casino edge to merely 0.5%
action player::getBasicStrategyAction(gamestate a) {
  
}

//player action is a function of the dealer's faceup card, the cards they were dealt, and the true count
action player::getAction(gamestate a) {
  //actions are derived by checking for deviations to basic strategy
  //and then applying basicStrategy

}
