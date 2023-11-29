#include "player.h"
#include "strategy.h"
#include <iostream>
#include <iomanip>

using namespace std;

player::player(){

}

card player::seeCard(card a) {
  cardsCounted++;
  vector<int>* thisCountingMethod;
  card perceivedCard = hisConfusionMatrix.perceive(a);
  cout << "Perceived " << getCardName(a) << " as " << getCardName(perceivedCard) << endl;
  thisCountingMethod = &g_countingMethods[hisCountingMethod];
  runningCount += thisCountingMethod->at(getEffectiveCard(perceivedCard));
  cout << "Done" << endl;
  return perceivedCard;
}

card player::seeCardP(card a) {
  cardsCounted++;
  vector<int>* thisCountingMethod;
  card perceivedCard = a;
  thisCountingMethod = &g_countingMethods[hisCountingMethod];
  runningCount += thisCountingMethod->at(getEffectiveCard(perceivedCard));
  return a;
}

float player::getBankroll() {
  return bankroll;
}

void player::getMoney(float a) {
  bankroll += a;
}

void player::loseMoney(float a) {
  bankroll -= a;
}

void player::setCountingMethod(countingMethod a) {
  hisCountingMethod = a;
}

void player::resetCount() {
  runningCount = 0;
  cardsCounted = 0;
}

void player::resetAll(){
  resetCount();
  bankroll = G_STARTING_BANKROLL;
}

//if the player doesn't have an advantage, do the minimum bet
int player::getBet() {
  float trueCount;

  //this is not quite how humans calculate decks remaining!
  //as such, it may be overestimating playeradvantage
  float decksRemaining = cardsCounted;
  decksRemaining = G_NUM_DECKS - (decksRemaining/52);

  trueCount = runningCount / decksRemaining;

  //cout << "RC: " << runningCount << endl;
  cout << "TC: " << trueCount << endl;
  cout << "Total counted: " << cardsCounted << endl;

  //playeradvantage is negative if the trueCount is below 1
  if(trueCount <= 1) {
    cout << "Player advantage: <0"  << endl;
    return G_MINIMUM_BET;
  } else {
    float playerAdvantage = (trueCount - 1)/200; //playeradvantage increases by 0.5% for every trueCount
    //if the player has an advantage, bet the advantage percentage of the bankroll (kelly criterion)
    float ret = playerAdvantage * bankroll;
    if(ret < G_MINIMUM_BET) {ret = G_MINIMUM_BET;}
    if(ret > G_MAXIMUM_BET) {ret = G_MAXIMUM_BET;}
    cout << "Player advantage: " << playerAdvantage << endl;
    return ret;
  }
}

//perfect basic strategy reduces casino edge to merely 0.5%
action player::getBasicStrategyAction(gamestate a) {
  
  return hit;
}

//player action is a function of the dealer's faceup card, the cards in this stack, and the true count
action player::getAction(gamestate g, int stackIndex) {
  float trueCount = getTrueCount();

  //did thisStack bust?
  if(getIdealCount(g.stacks[stackIndex]) > 21) {
    return stay;
  }

  //check for deviations
  action actionFromDeviations = getActionFromDeviations(g, stackIndex, trueCount);
  if(actionFromDeviations != voidaction) {
    cout << "Player took a deviation from basic strategy." << endl;
    //if the deviation tells us to split, don't take it
    if(actionFromDeviations != split) {
      return actionFromDeviations;
    }
  }

  if(shouldPlayerSurrender(g, stackIndex)){
    return surrender;
  }else{
    if(shouldUseHardTotals(g, stackIndex)){
      cout << "Player uses Hard Totals to decide." << endl;
      return getHardTotalsAction(g, stackIndex, trueCount);
    }else{
      cout << "Player uses Soft Totals to decide" << endl;
      return getSoftTotalsAction(g, stackIndex, trueCount);
    }
  }
  return getHardTotalsAction(g, 0, trueCount);
}

float player::getTrueCount() {
  float trueCount;

  float decksRemaining = cardsCounted;
  decksRemaining = G_NUM_DECKS - (decksRemaining/52);

  trueCount = runningCount / decksRemaining;
  return trueCount;
}

float player::getRunningCount() {
  return runningCount;
}

bool player::takesInsurance() {
  float tc = getTrueCount();
  return shouldPlayerInsure(tc);
}

void player::setConfusionMatrix(c_matrix a) {
  hisConfusionMatrix = a;
}

card player::perceive(card real) {
  return hisConfusionMatrix.perceive(real);
}
