#include "player.h"
#include "strategy.h"
#include <iostream>
#include <iomanip>

using namespace std;

player::player(){

}

void player::seeCard(card a) {
  cardsCounted++;
  vector<int>* thisCountingMethod;
  card perceivedCard = hisConfusionMatrix.perceive(a);
  //cout << "Perceived " << getCardName(a) << " as " << getCardName(perceivedCard) << endl;
  thisCountingMethod = &g_countingMethods[hisCountingMethod];
  runningCount += thisCountingMethod->at(getEffectiveCard(perceivedCard));
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
    return actionFromDeviations;
  }

  if(shouldPlayerSurrender(g, stackIndex)){
    return surrender;
  }else if(g.stacks[stackIndex].size() == 2 && getEffectiveCard(g.stacks[stackIndex][0]) == getEffectiveCard(g.stacks[stackIndex][1]) && shouldPlayerSplit(g, stackIndex)){
    return split;
  }else{
    if(shouldUseHardTotals(g, stackIndex)){
      cout << "useHardTotals" << endl;
      return getHardTotalsAction(g, stackIndex, trueCount);
    }else{
      cout << "use soft totals" << endl;
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

bool player::takesInsurance() {
  float tc = getTrueCount();
  return (tc > 3);
}

void player::setConfusionMatrix(c_matrix a) {
  hisConfusionMatrix = a;
}
