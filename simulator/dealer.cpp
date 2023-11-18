#include "common.h"
#include "shoe.h"
#include "dealer.h"
#include "strategy.h"
#include <iostream>

using namespace std;

dealer::dealer(){

}

dealer::dealer(shoe myShoe){
  setShoe(myShoe);
}

int dealer::playRound(player p) {
  gamestate g;
  //shuffle the decks if we have too few cards to play with
  if(herShoe.getCardsLeftInShoe() < G_MINIMUM_CARDS_IN_SHOE) {
    herShoe.reset();
    p.resetCount();
  }
  
  //bets are taken before dealing cards
  int playerBet = p.getBet();
  p.loseMoney(playerBet);

  card a = herShoe.drawCard();
  card b = herShoe.drawCard();
  card c = herShoe.drawCard();
  card d = herShoe.drawCard();
  g.stacks[0].push_back(a);
  g.stacks[0].push_back(b);
  g.dealersCards.push_back(c);
  g.dealersCards.push_back(d);
  p.seeCard(a);
  p.seeCard(b);
  p.seeCard(c);
  printPlayerCards(g);

  //check if the player got a blackjack
  if(isBlackjack(g.stacks[0][0], g.stacks[0][1])) {
    //payout is 3:2
    int profit = playerBet * 1.5;
    p.getMoney(playerBet + profit);
    return 0;
  }

  //check if the dealer got a blackjack
  if(isBlackjack(g.dealersCards[0],g.dealersCards[1])) {
    return 0;
  }

  //!!! see if you can do good here Saket

  for(int i = 0; i < g.stacks.size(); i++) {
    action playerAction = p.getAction(g, i);
  }


  p.seeCard(d);

  //if the player doesn't have enough money to keep playing, kick them out
  if(p.getBankroll() < G_MINIMUM_BET) {
    return 1;
  }


  return 0;
}

void dealer::setShoe(shoe myShoe){
  herShoe = myShoe;
}
