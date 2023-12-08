#include "analysis.h"
#include "common.h"
#include "matrix.h"
#include "player.h"
#include "shoe.h"
#include <iostream>
#include <string>
#include "strategy.h"
#include "dealer.h"
#include <random>
#include "argparse/argparse.hpp"

struct MyArgs : public argparse::Args {
    std::string& matrix_path = arg("The path to the confusion matrix to test");
    size_t& num_rounds = kwarg("rounds", "The number of rounds of blackjack to play per trial").set_default(800);
    size_t& num_trials = kwarg("trials", "The number of trials to run").set_default(10000);
    float& perfectness = kwarg("perfectness", "The interpolation value in the interval [0.0, 1.0] to the identity matrix").set_default(0.0);
    int& seed = kwarg("seed", "The seed to drive the RNG; a value of 0 will use a randomized seed").set_default(1);
};

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

    c_matrix m{args.matrix_path};
    m.perfectify(args.perfectness);
    analysis a{m, rng};
    a.runTrials(args.num_trials, args.num_rounds, rng);
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
