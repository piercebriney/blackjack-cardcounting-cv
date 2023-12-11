#ifndef STRATEGY
#define STRATEGY
#include "common.h"
#include <vector>
#include <string>
#include <unordered_map>

typedef std::vector<std::vector<std::string>> table;
typedef std::unordered_map<std::string, std::string> mp;

extern table g_hardTotalsTable;
extern table g_softTotalsTable;
extern table g_pairSplittingTable;
extern table g_lateSurrenderTable;
extern mp g_hardTotalsMap;
extern mp g_softTotalsMap;
extern mp g_pairSplittingMap;
extern mp g_lateSurrenderMap;
extern std::vector<deviation> g_illustrious_18;

void loadStrategy();

action getHardTotalsAction(gamestate& g, int stackIndex, float trueCount);

action getSoftTotalsAction(gamestate& g, int stackIndex, float trueCount);

bool shouldPlayerSurrender(gamestate& g, int stackIndex);

bool shouldPlayerInsure(float trueCount);

bool shouldPlayerSplit(gamestate& g, int stackIndex);

bool shouldPlayerTakeInsurance(float trueCount);

bool shouldUseHardTotals(gamestate& g, int stackIndex);

action getActionFromDeviations(gamestate& g, int stackIndex, float trueCount);

#endif
