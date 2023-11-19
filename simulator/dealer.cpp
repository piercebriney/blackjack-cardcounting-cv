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

int dealer::playRound(player* p) {
  gamestate g;

  //shuffle the decks if we have too few cards to play with
  if(herShoe.getCardsLeftInShoe() < G_MINIMUM_CARDS_IN_SHOE) {
    herShoe.reset();
    p->resetCount();
  }
  //bets are taken before dealing cards
  int playerBet = p->getBet();
  p->loseMoney(playerBet);
  cout << "Player's bet is " << playerBet << endl;

  card a = herShoe.drawCard();
  card b = herShoe.drawCard();
  card c = herShoe.drawCard();
  card d = herShoe.drawCard();
  vector<card> initCards;
  initCards.push_back(a);
  initCards.push_back(b);
  g.stacks.push_back(initCards);
  g.actions.push_back(hit);
  //g.stacks[0].push_back(a);
  //g.stacks[0].push_back(b);
  g.dealersCards.push_back(c);
  g.dealersCards.push_back(d);
  p->seeCard(a);
  p->seeCard(b);
  p->seeCard(c);


  //check if the player got a blackjack
  bool playerGotBJ = 0;
  bool dealerGotBJ = 0;
  if(isBlackjack(g.stacks[0][0], g.stacks[0][1])) {
    playerGotBJ = 1;
  }

  //check if the dealer got a blackjack
  if(isBlackjack(g.dealersCards[0],g.dealersCards[1])) {
    dealerGotBJ = 1;
  }
  if(playerGotBJ && !dealerGotBJ) {
    //payout is 3:2
    cout << "RESULT: Player got a blackjack." << endl;
    int profit = playerBet * 1.5;
    p->getMoney(playerBet + profit);
    return 0;
  }
  if(dealerGotBJ && !playerGotBJ) {
    cout << "RESULT: Dealer got a blackjack." << endl;
    return 0;
  }
  if(playerGotBJ && dealerGotBJ) {
    cout << "RESULT: Both player and dealer got blackjacks." << endl;
    p->getMoney(playerBet);
    return 0;
  }

  //!!! see if you can do good here Saket
  cout << "Dealer's card is " << getCardName(c) << endl;

  for(int i = 0; i < g.stacks.size(); i++) {
    bool handOver = false;
    while(!handOver){
      cout << "Player Cards are: ";
      printPlayerCards(g, i);
      action playerAction = p->getAction(g, i);
      if(playerAction == surrender){
        cout << "Player surrenders:" << endl; 
        g.actions[i] = surrender;
        p->getMoney(playerBet/2);
        handOver = true;
        
      }else if(playerAction == split){
        cout << "Player splits:" << endl; 
        p->loseMoney(playerBet);
        vector<card> stackFromSplit;
        card splitCard = g.stacks[i][0];
        card newCard = herShoe.drawCard();
        p->seeCard(newCard);
        g.stacks[i][0] = newCard;
        stackFromSplit.push_back(splitCard);
        g.stacks.push_back(stackFromSplit);
        g.actions.push_back(hit);
      }else if(playerAction == hit){
        cout << "Player hits" << endl; 
        card newCard = herShoe.drawCard();
        p->seeCard(newCard);
        g.stacks[i].push_back(newCard);
      }else if(playerAction == stay){
        cout << "Player stands" << endl;
        handOver = true;
        g.actions[i] = stay;
      }else if(playerAction == doubledown) {
        cout << "Player doubles down" << endl;
        card newCard = herShoe.drawCard();
        p->seeCard(newCard);
        g.stacks[i].push_back(newCard);
        handOver = true;
        p->loseMoney(playerBet);
        g.actions[i] = doubledown;
      }
    }
  }
  
  //dealer plays
  bool dealerStays = 0;
  int dealerSum = 0;
  for(auto c : g.dealersCards) {
    if(getEffectiveCard(c) == _A) {
      dealerSum += 11;
    } else {
      dealerSum += getEffectiveCardValue(c);
    }
  }
  while(dealerSum < 17) {
    dealerSum = 0;
    //cout << "Dealer draws a card" << endl;
    g.dealersCards.push_back(herShoe.drawCard());
    for(auto c : g.dealersCards) {
      if(getEffectiveCard(c) == _A) {
        dealerSum += 11;
      } else {
        dealerSum += getEffectiveCardValue(c);
      }
    }
  }

  int dealerIdealSum = getIdealCount(g.dealersCards);
  bool dealerBusts = (dealerIdealSum > 21);

  cout << "Dealer's cards sum to " << dealerSum << "." << endl;

  bool kill = 0;
  //determine result for each stackA
  for(int i = 0; i < g.stacks.size(); i++) {
    int multiplier = 1;
    if(g.actions[i] == doubledown) {
      multiplier = 2;
      g.actions[i] = stay;
    }
    int stackIdealSum = getIdealCount(g.stacks[i]);
    cout << "Stack " << i << " cards sum to " << stackIdealSum << endl;
    if(g.actions[i] == hit) { cout << "Stack actions not properly set" << endl; abort();}
    if(g.actions[i] == surrender) {
      //if the player won, they won't be paid
      cout << "RESULT: Stack " << i << "SURRENDERED." << endl;
    }
    if(g.actions[i] == stay) {
      int stackBusts = (stackIdealSum > 21);
      if(dealerBusts && !stackBusts) {
        cout << "RESULT: Stack " << i << " WON with multiplier " << multiplier << "." << endl;
        p->getMoney(playerBet*multiplier);
        int profit = playerBet;
        p->getMoney(profit*multiplier);
      } else if(stackBusts && !dealerBusts) {
        cout << "RESULT: Stack " << i << " LOST." << endl;

      } else if(stackBusts && dealerBusts) {
        p->getMoney(playerBet*multiplier);
        cout << "RESULT: Stack " << i << " TIED." << endl;
      } else { //neither busted
        //who has a higher total?
        if(stackIdealSum > dealerIdealSum) {
          cout << "RESULT: Stack " << i << " WON with multiplier " << multiplier << "." << endl;
          p->getMoney(playerBet*multiplier);
          int profit = playerBet;
          p->getMoney(profit*multiplier);
        } else if(dealerIdealSum > stackIdealSum) {
          cout << "RESULT: Stack " << i << " LOST." << endl;
          
        } else if(dealerIdealSum == stackIdealSum) {
          cout << "RESULT: Stack " << i << " TIED." << endl;
          p->getMoney(playerBet*multiplier);
        }
      }
    }
  }

  p->seeCard(d);

  //if the player doesn't have enough money to keep playing, kick them out
  if(p->getBankroll() < G_MINIMUM_BET) {
    return 1;
  }


  return 0;
}

void dealer::setShoe(shoe myShoe){
  herShoe = myShoe;
}
