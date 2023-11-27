#ifndef ANALYSIS
#define ANALYSIS
#include "matrix.h"
#include "player.h"
#include "shoe.h"
#include "strategy.h"
#include "dealer.h"

class analysis {
    private:
        player player1;
        shoe shoe1;
        dealer dealer1;
    public:
        analysis(c_matrix m);
        double getAverageProfit(int numTrials, int numRounds);

};

#endif