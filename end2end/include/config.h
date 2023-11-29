#include <vector>
#include "common.h"
#pragma once

#if 0
constexpr int NUM_LABELS = 52;
const std::vector<std::string> CLASS_NAMES = {"10C", "10D", "10H", "10S", "2C", "2D", "2H", "2S", "3C", "3D", "3H", "3S", "4C", "4D", "4H", "4S", "5C", "5D", "5H", "5S", "6C", "6D", "6H", "6S", "7C", "7D", "7H", "7S", "8C", "8D", "8H", "8S", "9C", "9D", "9H", "9S", "AC", "AD", "AH", "AS", "JC", "JD", "JH", "JS", "KC", "KD", "KH", "KS", "QC", "QD", "QH", "QS"};
cv::Size size{640, 640};
static card LABEL_TO_CARD[] = {_AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS, _AS};
#else
cv::Size size{1280, 1280};
constexpr int NUM_LABELS = 13;
const std::vector<std::string> CLASS_NAMES = {"ace",  "two",  "three",  "four",  "five",  "six",  "seven",  "eight",  "nine",  "ten",  "jack",  "queen",  "king"};
static card LABEL_TO_CARD[] = {_AS, _2S, _3S, _4S, _5S, _6S, _7S, _8S, _9S, _10S, _JS, _QS, _KS};
#endif
