#include "common.h"
#include "player.h"
#include "strategy.h"
#include "fp.h"
#include "bridge.h"
#include "config.h"
#pragma once

struct Game {
    gamestate g;
    player p;
    Game(): p{}, g{} {
        commonInit();
        loadStrategy();
        p.setCountingMethod(HiLo);
        // invariant: ensure perceivedStacks and stacks have 1 element
        g.perceivedStacks.emplace_back();
        g.stacks.emplace_back();
    }
    action get_action() {
        return p.getAction(g, 0);
    }
    void observe_player(card c) {
        auto x = p.seeCardP(c);
        g.perceivedStacks[0].push_back(x);
        g.stacks[0].push_back(x);
    }
    void observe_dealer(card c) {
        auto x = p.seeCardP(c);
        g.dealersPerceivedCards.push_back(x);
        g.dealersCards.push_back(x);
    }
    void reset_round() {
        g.perceivedStacks[0].clear();
        g.stacks[0].clear();
        g.dealersPerceivedCards.clear();
        g.dealersCards.clear();
    }
    bool is_actionable() {
        return g.perceivedStacks[0].size() >= 2;
    }
    void process(FrameProcessor& fp, Bridge& b) {
        fp.dealer_ticks.extract([&](int label) {
            card c = LABEL_TO_CARD[label];
            observe_dealer(c);
        });
        fp.player_ticks.extract([&](int label) {
            card c = LABEL_TO_CARD[label];
            observe_player(c);
            if (is_actionable()) {
                action a = get_action();
                b.send_action(a);
            }
        });
    }
};
