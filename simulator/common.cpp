#include "common.h"

effectiveCard getEffectiveCard(cardName a) {
  int rank = a % 13;
  effectiveCard ret;
  if(rank < 9) {
    ret = (effectiveCard)(rank + 2);
  } else if(rank < 12) {
    ret = _F;
  } else {
    ret = _A;
  }
  return ret;
}

