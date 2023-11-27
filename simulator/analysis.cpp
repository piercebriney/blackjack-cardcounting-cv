#include "analysis.h"
#include <iostream>
using namespace std;

analysis::analysis(c_matrix mat){
    player1.setCountingMethod(HiOpt2);
    player1.setConfusionMatrix(mat);
    shoe1.reset();
    dealer1.setShoe(shoe1);
}

double analysis::getAverageProfit(int numTrials, int numRounds){
    double totalProfit = 0;
    for(int t = 0; t < numTrials; t++){
        player1.resetAll();
        int initialBankRoll = player1.getBankroll();
        cout << "Initial Bankroll: " << initialBankRoll << endl;
        for(int i = 0; i < numRounds; i++){
            dealer1.playRound(&player1);
        }
        totalProfit += player1.getBankroll() - initialBankRoll;
        cout << "Player profit in trial #" << numTrials+1 << ": " << player1.getBankroll()-initialBankRoll << endl; 
    }
    cout << "Player profit average over " << numTrials << " trials is " << totalProfit/numTrials << endl;
    return totalProfit/numTrials;
}
