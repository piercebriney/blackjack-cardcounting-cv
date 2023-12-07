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
        void runTrials(int numTrials, int numRounds, Rng& rng, std::vector<float>& profits);
        void printStats(std::vector<float>& profits);
        float calcStandDev(std::vector<float>& profits);
        void testEpsilons(std::string filestr, Rng& rng);
        void varyConfusionMatrices(std::vector<c_matrix>& mats, int numTrials, int numRounds, Rng& rng);

};

#endif