#include "common.h"
#include "matrix.h"
#include "player.h"
#include "shoe.h"
#include <iostream>
#include <string>
#include "strategy.h"
#include "dealer.h"
#include <random>
#include <vector>
#include <omp.h>
#include "argparse/argparse.hpp"
#include <cstdio>

struct MyArgs : public argparse::Args {
    std::vector<std::string>& matrix_paths = arg("The path(s) to the confusion matrix to test, space-separated").multi_argument();
    size_t& num_rounds = kwarg("rounds", "The number of rounds of blackjack to play per trial").set_default(800);
    size_t& num_trials = kwarg("trials", "The number of trials to run").set_default(10000);
    float& perfectness = kwarg("perfectness", "The interpolation value in the interval [0.0, 1.0] to the identity matrix").set_default(0.0);
    int& decks = kwarg("decks", "The number of decks").set_default(G_NUM_DECKS);
    int& seed = kwarg("seed", "The seed to drive the RNG; a value of 0 will use a randomized seed").set_default(1);
    long& min_bet = kwarg("min", "The minimum betting amount in dollars").set_default(G_MINIMUM_BET);
    long& max_bet = kwarg("max", "The maximum betting amount in dollars").set_default(G_MAXIMUM_BET);
    long& starting_bankroll = kwarg("bankroll", "The starting bankroll amount in dollars").set_default(G_STARTING_BANKROLL);
    long& rounds_per_hour = kwarg("rate", "The amount of rounds played per hour").set_default(G_NUM_ROUNDS_PER_HOUR);
    bool& save = flag("save", "Save the raw profits to a file named <matrix path>.output").set_default(false);
};

void save_profits(std::string& path, std::vector<long>& profits) {
    auto output = path + ".output";
    FILE* f = fopen(output.c_str(), "w");
    if (!f) {
        throw std::runtime_error{"unable to open save file"};
    }
    for (auto p: profits) {
        fprintf(f, "%ld\n", p);
    }
    fclose(f);
}

// https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
bool ends_with(std::string& s, std::string& ending) {
    if (s.length() >= ending.length()) {
        return (0 == s.compare(s.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void _main(MyArgs& args) {
    Rng rng;
    if (args.seed == 0) {
        pcg_extras::seed_seq_from<std::random_device> seed_source;
        rng = Rng{seed_source};
    } else {
        rng = Rng{(__int128 unsigned)args.seed};
    }

    commonInit();
    loadStrategy();

    std::string ending{".txt"};

    for (auto& matrix_path: args.matrix_paths) {
        if (!ends_with(matrix_path, ending)) {
            continue;
        }
        std::cout << "Loading " << matrix_path << std::endl;
        c_matrix m{matrix_path};
        m.perfectify(args.perfectness);
        
        long sum = 0;
        long squares = 0;
        long positives = 0;
        std::vector<long> profits;
        profits.resize(args.num_trials);

        #pragma omp parallel
        {
            int stream = omp_get_thread_num();

            Rng copy{rng};
            pcg64 temp_rng{copy(), (__int128 unsigned)stream};
            Rng r{(__int128 unsigned)temp_rng() << 64 | (__int128 unsigned)temp_rng()};

            gamestate g;
            player p{args.decks, args.starting_bankroll, args.min_bet, args.max_bet};
            p.setConfusionMatrix(m);
            p.setCountingMethod(HiLo);
            shoe s{args.decks};
            s.reset(r);
            dealer d{s};

            #pragma omp for reduction(+:sum, squares, positives)
            for (size_t t = 0; t < args.num_trials; t++) {
                p.resetAll();
                s.reset(r);
                long initialBankRoll = p.getBankroll();
                for (size_t i = 0; i < args.num_rounds; i++){
                    try {
                        d.playRound(p, g, r, false);
                    } catch (std::runtime_error& e) {
                        // ran out of cards
                        break;
                    }
                }
                long profit = p.getBankroll() - initialBankRoll;
                sum += profit;
                squares += profit * profit;
                positives += (long)(profit > 0);
                profits[t] = profit;
            }
        }
        double mean = (double)sum / args.num_trials;
        double variance = (double)squares / args.num_trials - mean * mean;
        double stdev = sqrt(variance * ((double)args.num_trials / (args.num_trials - 1)));
        double prob = (double)positives / args.num_trials;
        printf("Mean: %.2f\nStandard deviation: %.2f\nProbability of profit: %.2f\n\n", mean, stdev, prob);
        if (args.save) {
            save_profits(matrix_path, profits);
        }
    }
}

int main(int argc, char* argv[]) {
    try {
        auto args = argparse::parse<MyArgs>(argc, argv, true);
        _main(args);
    } catch (const std::runtime_error &e) {
        std::cerr << "error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
