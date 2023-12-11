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
    void reset_shoe() {
        reset_round();
        p.resetAll();
    }
    bool empty() {
        return g.dealersCards.empty() && g.stacks[0].empty();
    }
    bool is_actionable() {
        return g.perceivedStacks[0].size() >= 2 && g.dealersCards.size() == 1;
    }
    bool is_actionable_loose() {
        return g.perceivedStacks[0].size() >= 2 && g.dealersCards.size() > 0;
    }
    void process(FrameProcessor& fp, Bridge& b) {
        fp.extract([&](Object& o, bool dealer) {
            card c = LABEL_TO_CARD[o.label];
            const char* who = dealer ? "DEALER" : "PLAYER";
            if (dealer) {
                observe_dealer(c);
            } else {
                observe_player(c);
            }
            auto s = getEffectiveCardName(getEffectiveCard(c));
            printf("%s: %s\n", who, s.data());
            if (is_actionable()) {
                action a = get_action();
                auto s = getActionName(a);
                printf("ACTION: %s\n", s.data());
                b.send_action(a);
            }
        });
    }
};
