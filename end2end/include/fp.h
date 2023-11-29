#include "common.hpp"
#include "opencv2/opencv.hpp"
#include <array>
#include <algorithm>
#include "config.h"
#pragma once

constexpr int FRAME_THRESHOLD = 19;
constexpr int FRAME_MAX = 20;
constexpr float IOU_THRESHOLD = 0.90;

bool check_iou(cv::Rect_<float>& a, cv::Rect_<float>& b) {
    auto intersect = (a & b).area();
    // avoid division
    return intersect >= IOU_THRESHOLD * (a.area() + b.area() - intersect);
}

struct FrameProcessor {
    std::vector<Object> objs;
    std::vector<Object> tmp;
    void process(std::vector<Object>& new_objs) {
        tmp.clear();
        for (auto& o: objs) {
            o.tick -= 1;
        }
        for (auto& n: new_objs) {
            auto it = std::find_if(objs.begin(), objs.end(), [&](Object& o) {
                return o.label == n.label && check_iou(o.rect, n.rect);
            });
            if (it != objs.end()) {
                auto& found = *it;
                found.tick = std::max(found.tick + 2, FRAME_MAX);
                // use latest bounding box
                found.rect = n.rect;
            } else {
                tmp.push_back(n);
            }
        }
        // append tmp to objs
        objs.reserve(objs.size() + tmp.size());
        objs.insert(objs.end(), tmp.begin(), tmp.end());

        // remove dead objs
        auto new_end = std::remove_if(objs.begin(), objs.end(), [](const Object& o) {
            return o.tick <= 0;
        });
        objs.erase(new_end, objs.end());
    }
    template<typename F>
    void extract(F&& f) {
        for (auto& o: objs) {
            if (o.tick >= FRAME_THRESHOLD && !o.seen) {
                f(o, in_top_half(o.rect, SIZE.height));
                o.seen = true;
            }
        }
    }
    void clear() {
        objs.clear();
        tmp.clear();
    }
};

