#include "player.h"

using namespace std;

void player::seeCard() {
  switch(hisCountingMethod) {
    case HiLo:

      break;
  }
}

float player::getBankroll() {
  return bankroll;
}

void player::setCountingMethod(countingMethod a) {
  hisCountingMethod = a;
}
