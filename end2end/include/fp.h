#include "common.hpp"
#include "opencv2/opencv.hpp"
#include <array>
#include <algorithm>
#include "config.h"
#pragma once

constexpr int FRAME_THRESHOLD = 15;
constexpr int FRAME_MAX = 20;

struct Ticks {
    std::array<int, NUM_LABELS> ticks{};
    std::array<bool, NUM_LABELS> seen{};
    void dec() {
        for (int i = 0; i < NUM_LABELS; ++i) {
            ticks[i] = std::max(ticks[i] - 1, 0);
        }
    }
    void inc(int idx) {
        ticks[idx] = std::min(ticks[idx] + 2, FRAME_MAX);
    }

    template<typename F>
    void extract(F&& f) {
        for (int i = 0; i < NUM_LABELS; ++i) {
            if (ticks[i] > FRAME_THRESHOLD && !seen[i]) {
                f(i);
                seen[i] = true;
            }
        }
    }
    void clear() {
        ticks.fill(0);
        seen.fill(false);
    }
    void debug() {
        for (int i = 0; i < NUM_LABELS; ++i) {
            std::cout << i << " " << ticks[i] << std::endl;
        }
    }
};

struct FrameProcessor {
    Ticks player_ticks{};
    Ticks dealer_ticks{};
    cv::Size size;
    FrameProcessor(cv::Size& s) : size{s} {}

    void process(std::vector<Object>& objs) {
        std::array<Object, NUM_LABELS> best{};
        dealer_ticks.dec();
        player_ticks.dec();
        // deduplicate cards, keeping most confident result (2 boxes for 1 card if the model looks at the corners)
        for (auto& o: objs) {
            if (o.prob > best[o.label].prob) {
                best[o.label] = o;
            }
        }
        // increment ticks of detected cards
        for (auto& o: best) {
            if (o.prob != 0) {
                if (in_top_half(o.rect, size.height)) {
                    dealer_ticks.inc(o.label);
                } else {
                    player_ticks.inc(o.label);
                }
            }
        }
    }
    void clear() {
        player_ticks.clear();
        dealer_ticks.clear();
    }
};

