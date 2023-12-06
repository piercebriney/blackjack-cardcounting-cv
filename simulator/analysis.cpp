#include "analysis.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <omp.h>
using namespace std;

analysis::analysis(c_matrix& mat, Rng& rng){
    player1.setCountingMethod(HiOpt2);
    player1.setConfusionMatrix(mat);
    shoe1.reset(rng);
    dealer1.setShoe(shoe1);
}

void analysis::runTrials(int numTrials, int numRounds, Rng& rng, vector<float>& profits){
    profits.resize(numTrials);

    #pragma omp parallel
    {
        int stream = omp_get_thread_num();
        gamestate g;
        player p{this->player1};
        shoe s{this->shoe1};
        dealer d{this->dealer1};
        Rng copy{rng};
        pcg64 temp_rng{copy(), (__int128 unsigned)stream};
        Rng r{(__int128 unsigned)temp_rng() << 64 | (__int128 unsigned)temp_rng()};

        #pragma omp for
        for (size_t t = 0; t < numTrials; t++) {
            p.resetAll();
            s.reset(r);
            int initialBankRoll = p.getBankroll();
            for(int i = 0; i < numRounds; i++){
                d.playRound(p, g, r, false);
            }
            float profit = p.getBankroll() - initialBankRoll;
            profits[t] = profit;
        }
    }
}

void analysis::printStats(vector<float>& profits){
    float sum = 0.0;
    float squares = 0.0;
    size_t n = profits.size();

    #pragma omp parallel for reduction (+:sum)
    for (size_t i = 0; i < n; ++i) {
        sum = sum + profits[i];
    }

    float mean = sum / n;

    #pragma omp parallel for reduction (+:squares)
    for (size_t i = 0; i < n; i++){
        float diff = profits[i] - mean;
        float square = diff * diff;
        squares = squares + square;
    }
    float variance = squares / (n - 1);
    float stdev = sqrt(variance);
    printf("mean %f stdev %f\n", mean, stdev);
}

double analysis::getAverageProfit(int numTrials, int numRounds, Rng& rng){
    float totalProfit = 0;
    vector<float> profits;
    gamestate g;
    for(int t = 0; t < numTrials; t++){
        // cout << "TRIAL " << t << '\n';
        player1.resetAll();
        shoe1.reset(rng);
        int initialBankRoll = player1.getBankroll();
        //cout << "Initial Bankroll: " << initialBankRoll << endl;
        for(int i = 0; i < numRounds; i++){
            dealer1.playRound(player1, g, rng, false);
        }
        float p = player1.getBankroll() - initialBankRoll;
        //cout << "After bankroll: " << player1.getBankroll() << endl;
        totalProfit += p;
        profits.push_back(p);
        //cout << "Player profit in trial #" << t+1 << ": " << player1.getBankroll()-initialBankRoll << endl; 
    }
    
    double avgProfit = totalProfit/numTrials;
    cout << "Player profit average over " << numTrials << " trials is " << totalProfit/numTrials << endl;
    cout << "Player hourly wage over " << numTrials << ": " << avgProfit / (G_NUM_ROUNDS / G_NUM_ROUNDS_PER_HOUR) << endl;
    cout << "Standard Deviation is " << calcStandDev(profits) << endl;
    return totalProfit/numTrials;
}



float analysis::calcStandDev(vector<float>& profits){
    float sum = 0.0;
    float mean = 0.0;
    float variance = 0.0;

    for(float profit : profits){
        sum += profit;
    }
    mean = sum/profits.size();

    for(int i = 0; i < profits.size(); i++){
        variance += pow(profits[i] - mean, 2);
    }
    variance /= profits.size();

    return sqrt(variance);

}

void analysis::testEpsilons(string filestr, Rng& rng){
    vector<float> profits;
    fstream fout;
    fout.open("results/epstest.csv", ios::out | ios::app);
    for(double i = 0; i <= 1.00; i += 0.1){
        c_matrix myMatrix = c_matrix(filestr);
        myMatrix.perfectify(i);
        player1.setConfusionMatrix(myMatrix);
        float p = getAverageProfit(1000, G_NUM_ROUNDS, rng);
        //cout << "Average profit for matrix with epsilon " << i << " is " << p;
        fout << i << "," << p << "\n";
        profits.push_back(p);
    }
    for (int i = 0; i < profits.size(); i++){
        cout << profits[i] << endl;
    }
}

void analysis::varyConfusionMatrices(vector<c_matrix>& mats, int numTrials, int numRounds, Rng& rng) {
    for(int i = 0; i < mats.size(); i++){
        player1.setConfusionMatrix(mats[i]);
        int x = getAverageProfit(numTrials, numRounds, rng);
    }
}