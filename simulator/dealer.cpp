#include "common.h"
#include "shoe.h"
#include "dealer.h"


int dealer::playRound(player p) {
  gamestate g;

  //shuffle the decks if we have too few cards to play with
  if(herShoe.getCardsLeftInShoe() < G_MINIMUM_CARDS_IN_SHOE) {
    herShoe.reset();
    p.resetCount();
  }

  //bets are taken before dealing cards
  int playerBet = p.getBet();

  cardName a = herShoe.drawCard();
  cardName b = herShoe.drawCard();
  cardName c = herShoe.drawCard();
  cardName d = herShoe.drawCard();
  g.playersCards.push_back(a);
  g.playersCards.push_back(b);
  g.dealersCards.push_back(c);
  g.dealersCards.push_back(d);
  p.seeCard(a);
  p.seeCard(b);
  p.seeCard(c);

  action playerAction = p.getAction(g);
  for(;;) {
    if(playerAction == stay || playerAction == surrender) {
      break;
    }
    if(playerAction == hit) {
      cardName e = herShoe.drawCard();
      p.seeCard(e);
      g.playersCards.push_back(e);
    }
    playerAction = p.getAction(g);
  }

  p.seeCard(d);

  //if the player doesn't have enough money to keep playing, kick them out
  if(p.getBankroll() < G_MINIMUM_BET) {
    return 1;
  }

  return 0;
}