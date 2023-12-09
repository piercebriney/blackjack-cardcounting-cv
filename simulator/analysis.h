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
        analysis(c_matrix& m, Rng& rng);
        double getAverageProfit(int numTrials, int numRounds, Rng& rng);
        void runTrials(size_t numTrials, size_t numRounds, Rng& rng);
        void testEpsilons(std::string filestr, Rng& rng);
        void varyConfusionMatrices(std::vector<c_matrix>& mats, int numTrials, int numRounds, Rng& rng);

};

#endif