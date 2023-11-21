#ifndef STRATEGY
#define STRATEGY
#include "common.h"
#include <vector>
#include <string>

typedef std::vector<std::vector<std::string>> table;

extern table g_hardTotalsTable;
extern table g_softTotalsTable;
extern table g_pairSplittingTable;
extern table g_lateSurrenderTable;
extern std::vector<deviation> g_illustrious_18;

std::string lookup(table table, std::string column, std::string row);

void loadStrategy();

action getHardTotalsAction(gamestate g, int stackIndex, float trueCount);

action getSoftTotalsAction(gamestate g, int stackIndex, float trueCount);

bool shouldPlayerSurrender(gamestate g, int stackIndex);

bool shouldPlayerSplit(gamestate g, int stackIndex);

bool shouldPlayerTakeInsurance();

bool shouldUseHardTotals(gamestate g, int stackIndex);

action getActionFromDeviations(gamestate g, int stackIndex, float trueCount);

#endif
