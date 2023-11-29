#include <chrono>
#pragma once

using std::chrono::system_clock;
using std::chrono::time_point;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::duration_cast;

struct Timer {
    time_point<system_clock> start;
    void reset() {
        start = system_clock::now();
    }
    int elapsed_ms() {
        auto end = system_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
};

