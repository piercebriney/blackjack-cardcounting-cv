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
  vector<card> initCards;
  initCards.push_back(a);
  initCards.push_back(b);
  g.stacks.push_back(initCards);
  //g.stacks[0].push_back(a);
  //g.stacks[0].push_back(b);
  g.dealersCards.push_back(c);
  g.dealersCards.push_back(d);
  p.seeCard(a);
  p.seeCard(b);
  p.seeCard(c);


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
  cout << "Dealer up card is " << getCardName(c) << endl;

  for(int i = 0; i < g.stacks.size(); i++) {
    bool handOver = false;
    while(!handOver){
      cout << "Player Cards are: ";
      printPlayerCards(g, i);
      action playerAction = p.getAction(g, i);
      if(playerAction == surrender){
        cout << "Player decides to surrender:" << endl; 
        p.getMoney(playerBet/2);
        handOver = true;
      }else if(playerAction == split){
        cout << "Player decides to split:" << endl; 
        p.loseMoney(playerBet);
        vector<card> stackFromSplit;
        card splitCard = g.stacks[i][0];
        g.stacks[i][0] = herShoe.drawCard();
        stackFromSplit.push_back(splitCard);
        stackFromSplit.push_back(splitCard);
        g.stacks.push_back(stackFromSplit);
      }else if(playerAction == hit){
        cout << "Player decides to hit" << endl; 
        g.stacks[i].push_back(herShoe.drawCard());
      }else if(playerAction == stay){
          cout << "Player decides to stand" << endl;
          handOver = true;
      }
    }
  }

  //TODO: Dealer plays and payout bets

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
