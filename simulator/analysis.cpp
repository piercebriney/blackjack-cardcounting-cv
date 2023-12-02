#include "analysis.h"
#include <iostream>
#include <cmath>
using namespace std;

analysis::analysis(c_matrix mat){
    player1.setCountingMethod(HiOpt2);
    player1.setConfusionMatrix(mat);
    shoe1.reset();
    dealer1.setShoe(shoe1);
}

double analysis::getAverageProfit(int numTrials, int numRounds){
    float totalProfit = 0;
    vector<float> profits;
    for(int t = 0; t < numTrials; t++){
        player1.resetAll();
        shoe1.reset();
        int initialBankRoll = player1.getBankroll();
        //cout << "Initial Bankroll: " << initialBankRoll << endl;
        for(int i = 0; i < numRounds; i++){
            dealer1.playRound(&player1);
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

float analysis::calcStandDev(vector<float> profits){
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

void analysis::testEpsilons(string filestr){
    vector<float> profits;
    for(double i = 0; i <= 1.00; i += 0.05){
        c_matrix myMatrix = c_matrix(filestr);
        myMatrix.perfectify(i);
        player1.setConfusionMatrix(myMatrix);
        float p = getAverageProfit(100, G_NUM_ROUNDS);
        cout << "Average profit for matrix with epsilon " << i << " is " << p;
        profits.push_back(p);
    }
    for (int i = 0; i < profits.size(); i++){
        cout << profits[i] << endl;
    }
}