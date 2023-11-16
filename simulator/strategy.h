#ifndef STRATEGY
#define STRATEGY
#include "common.h"
#include <vector>
#include <string>

typedef std::vector<std::vector<std::string>> table;

extern table g_hardTotalsTable;

std::string lookup(table table, std::string column, std::string row);

void loadStrategy();

action getHardTotalsAction(gamestate g);

#endif
