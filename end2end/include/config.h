#include <vector>
#include "common.h"
#pragma once

cv::Size SIZE{1280, 1280};
const std::vector<std::string> CLASS_NAMES = {"ace",  "two",  "three",  "four",  "five",  "six",  "seven",  "eight",  "nine",  "ten",  "jack",  "queen",  "king"};
static card LABEL_TO_CARD[] = {_AS, _2S, _3S, _4S, _5S, _6S, _7S, _8S, _9S, _10S, _JS, _QS, _KS};
