# Technical Notes

## Files and Their Purposes

analysis.py - Useful for running many trials and producing analytical graphs.

common.h/cpp - Defines shared objects/functions such as different enumerators for cards, a gamestate struct, and lots of helper functions to do common tasks, such as convert between card enums or return a card's name.

dealer.h/cpp - Defines a virtual dealer object, notably with the function dealer::playRound(4), which takes a player object and a gamestate object and plays a virtual round, taking/awarding money to the player based on the player's decisionmaking.

main.cpp - Uses OpenMP to parallelize execution of virtual rounds of blackjack. Takes command-line params for the path to a confusion matrix, the rounds per trial, the trials, the perfectify constant, the number of decks in a shoe, the min and max bet sizes, the player's starting bankroll, and an option to save the output to the disk. 

matrix.h/cpp - Defines a class representing a confusion matrix. Its structure is based off of the classes in the final version of the computer vision model, which is different from the cards which are effectively unique in blackjack, which is different from the cards which are physically unique, hence, the reason for three different enumerators.

params.h - Contains constants which affect how the simulator runs, but is largely overshadowed by the command-line parameters exposed to the user in the final executable.

player.h/cpp - Defines a virtual player, who has a bankroll and refers to strategy to make decisions.

shoe.h/cpp - Defines a virtual shoe, which contains some number of decks of cards, can be reset, and drawn from.

strategy.h/cpp - Contains tables of Basic Strategy and Deviations which the player object references. 
