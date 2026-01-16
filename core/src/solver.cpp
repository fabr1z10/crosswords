#include "core/solver.h"
#include <iostream>
#include <stack>
#include <algorithm>
#include "core/random.h"









void Solver::solve(Grid &g, Dictionary &dict, SeedMode mode, uint32_t seed) {
    // algorithm

    // starts with definition with most intersections
    // we need a tree of decisions

    //std::stack<std::shared_ptr<Decision>> decisions;

    DecisionTree tree(g, dict, mode, seed);
    tree.process();
    //decisions.push(std::make_shared<Decision>(slot, g, dict));
//    std::unordered_set<Slot*> decisionsStacked;
//    decisionsStacked.insert(slot);
//    int i = 0;
//    while (true) {
//
//        auto* decision = decisions.getNext();
//        auto result = decision->chooseWord();
//        if (result == 0) {
//            Slot *best = nullptr;
//            int weight = 0;
//
//            for (const auto& d : decisionsStacked) {
//                auto adj = g.getAdjacentSlots(d);
//                // now find next slot and push it on the stack. We look for ALL decisions stacked
//                for (const auto a : adj) {
//                    if (decisionsStacked.count(a.first) == 0 && a.second > weight) {
//                        best = a.first;
//                        weight = a.second;
//                    }
//                }
//            }
//            if (best == nullptr) {
//                std::cout << " don't know hot to proceed.\n";
//                exit(1);
//            }
//            std::cout << "next: " << best->toString() << " weight = " << weight << "\n";
//            decisions.push(std::make_shared<Decision>(best, g, dict));
//            decisionsStacked.insert(best);
//        } else {
//            std::cout << "not found. Backtracking.\n";
//            decisionsStacked.erase(decisions.top()->slot());
//            decisions.pop();
//        }
//        if (i == 150) {
//            break;
//        }
//        i++;
//    }
}