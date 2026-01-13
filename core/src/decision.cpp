#include "core/decision.h"
#include "core/random.h"
#include <list>
#include <chrono>
#include <iostream>
#include <algorithm>

//Decision::Decision(Slot *slot, Grid& grid, Dictionary& dict) : _slot(slot), _grid(grid), _dict(dict){
//    _intersections = _grid.getIntersectingSlots(slot);
//    words = dict.find(grid.getWord(slot));
//    std::cout << " -- Created node " << slot->toString() << ", available words: " << words.size() << "\n";
//    //_currentState = _grid.getWord(_slot);
//}

//void Decision::findValidWords() {
//    std::cout << " -- filtering words. Total words: " << words.size() << "\n";
//    while (!words.empty()) {
//        // get random word using swap and pop
//        std::string testWord = words.back();
//        words.pop_back();
//        _grid.setWord(_slot, testWord);
//        bool feasible = true;
//        for (const auto &intersection : _intersections) {
//            auto pattern = _grid.getWord(intersection.get());
//            auto wordsInt = _dict.find(pattern);
//            if (wordsInt.empty()) {
//                int posu = 0;
//                if (_slot->d == Direction::ACROSS) {
//                    posu = intersection->x - _slot->x;
//                } else {
//                    posu = intersection->y - _slot->y;
//                }
//                //removeStringsAtPosition(words, testWord[posu], posu);
//                feasible = false;
//                break;
//            }
//        }
//        _grid.clearWord(_slot);
//        if (feasible) {
//            valid.push_back(testWord);
//        }
//    }
//    std::cout << " -- ok words: " << valid.size() << "\n";
//
//
//}
//
//void Decision::addChild(std::shared_ptr<Decision> d) {
//    _children.push_back(d);
//    d->setParent(this);
//}
//
//int Decision::chooseWord() {
//    bool wordFound{false};
//    int trials=0;
//    while (!wordFound && !words.empty()) {
//
//        // get random word using swap and pop
//        int randomIndex = getRandomNumber(words.size()-1);
//        std::string testWord = words[randomIndex];
//        std::swap(words[randomIndex], words.back());
//        words.pop_back();
//
//        _grid.setWord(_slot, testWord);
//        if (testWord == "ceo") {
//            std::cout << "qui\n";
//        }
//        //std::cout << " * selected test word: " << testWord << "\n";
//        bool unfeasible = false;
//        for (const auto &intersection : _intersections) {
//            auto pattern = _grid.getWord(intersection.get());
//            std::vector<std::string> wordsInt;
//            //bool unf = _grid.isUnfeasible(intersection.get(), pattern);
//            //if (!unf) {
//            wordsInt = _dict.find(pattern);
//            //
//            //std::cout << "   * check " << intersection->toString() << " with " << pattern << ": " << n << "\n";
//            if (wordsInt.empty()) {
//                _ints.clear();
//                _grid.clearWord(_slot);
//                int posu = 0;
//                if (_slot->d == Direction::ACROSS) {
//                    posu = intersection->x - _slot->x;
//                } else {
//                    posu = intersection->y - _slot->y;
//                }
//                // drop all words with character testWord[posu] in posu
//                int before = words.size();
//                //removeStringsAtPosition(words, testWord[posu], posu);
//                int after = words.size();
//                //std::cout << "found an unfeasibility in " << posu << testWord[posu] <<", removed " << before-after << " words.\n";
//
//                unfeasible = true;
//                break;
//            } else {
//                _ints[intersection.get()] = wordsInt;
//                //std::cout << " intersection " << intersection.get()->toString() << " has " << wordsInt.size() << " words.\n";
//            }
//        }
//        trials++;
//        if (!unfeasible) {
//            wordFound = true;
//            _currentWord = testWord;
//        }
//
//    }
//    if (wordFound) {
//        //std::cout << " --- selected word: " << _currentWord << "; words left: " << words.size() << "\n";
//        return 0;
//    }
//    return 1;
//}

void DecisionTree::removeStringsAtPosition(std::vector<std::string>& vec, char c, size_t n) {
    std::cout << " Removing words with " << c << " @ " << n << "-th position.\n";
    std::cout << " # strings before removal: " << vec.size() << "\n";
    std::vector<std::string> result;
    std::copy_if(vec.begin(), vec.end(), std::back_inserter(result), [c, n](const std::string& str) {
        return n >= str.size() || str[n] != c;  // Only keep strings where the character at nth position is not c
    });
    std::cout << " # strings after removal: " << result.size() << "\n";
    vec = std::move(result);  // Replace the original vector with the filtered result
}

DecisionTree::DecisionTree(Grid &grid, Dictionary &dict) : _grid(grid), _dict(dict) {

    auto slot = _grid.peekSlot();
    //add(std::make_shared<Decision>(slot, _grid, _dict), nullptr);

    // initialize available words
    for (const auto& s : _grid.getSlots()) {
        _availableWords[s.get()] = _dict.find(_grid.getWord(s.get()));
    }

    for (const auto& aw : _availableWords) {
        std::cout << aw.first->toString() << ": " << aw.second.size() << "\n";
    }
}

//void DecisionTree::add(std::shared_ptr<Decision> decision, std::shared_ptr<Decision> parent) {
//    if (parent != nullptr) {
//        parent->addChild(decision);
//    } else {
//        _root = decision;
//    }
//    _nodes.insert({decision->slot(), decision});
//    _leaves.push_back(decision);
//}



int DecisionTree::process() {

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Slot*> wip;
    std::unordered_set<Slot*> decise;
    wip.push_back(_grid.peekSlot());
    while (true) {

        auto node = wip.back();
        std::cout << " Processing " << node->toString() << "\n";
        // get random word using swap and pop
        auto& words = _availableWords.at(node);
        if (node->number == 1 && node->d == Direction::DOWN) {
            std::cout << "figa";
        }
        std::cout << " Available words: " << words.size() << "\n";
        bool wordFound = false;
        while (!wordFound && !words.empty()) {
            int randomIndex = getRandomNumber(words.size() - 1);
            std::string testWord = words[randomIndex];
            std::swap(words[randomIndex], words.back());
            words.pop_back();
            _grid.setWord(node, testWord);
            // ora testa la parola, prendi tutte le intersezioni e calcola le parole che rimangono
            std::unordered_map<Slot*, std::vector<std::string>> cw;
            bool unfeasible = false;
            for (const auto& intersection : _grid.getIntersectingSlots(node)) {
                if (decise.count(intersection) == 0) {
                    auto iw = _dict.find(_grid.getWord(intersection));
                    if (iw.size() == 0) {
                        unfeasible = true;
                        auto unfeasiblePattern = _grid.getWord(intersection);
                        std::cout << " found unfeasibility at " << intersection->toString() << " with "
                                  << unfeasiblePattern << "\n";
                        auto index = node->d == Direction::ACROSS ? (intersection->x - node->x) : (intersection->y -
                                                                                                   node->y);
                        removeStringsAtPosition(_availableWords.at(node), testWord[index], index);
                        break;
                    }
                    cw[intersection] = iw;
                }
            }
            if (!unfeasible) {
                wordFound = true;
                std::cout << "Selected word: " << testWord << "\n";
                decise.insert(node);
                for (const auto& m : cw) {
                    _availableWords[m.first] = m.second;
                }
            }
        }
        if (!wordFound) {
            // need to pop from wip until I find a slot intersecting the unfeasible slot
            // pop unfeasible slot
            wip.pop_back();
            _grid.clearWord(node);
            decise.erase(node);
//            while (!_grid.areIntersecting(node, wip.back())) {
//                // back is not intersecting, pop it
//            }


            for (const auto &intersection : _grid.getIntersectingSlots(node)) {
                if (decise.count(intersection) == 0) {
                    if (!_grid.isComplete(intersection)) {
                        _availableWords[intersection] = _dict.find(_grid.getWord(intersection));
                    }
                }
            }
            if (!wip.empty()) {
                std::cout << " handling unfeasibility at " << node->toString() << ", word: " << _grid.getWord(node) << "\n";
                std::cout << " backtracking to " << wip.back()->toString() << "\n";

                // i can safely remove all available words
                int index = _grid.getIndex(wip.back(), node);
                auto word = _grid.getWord(wip.back());
                //std::cout << " can remove all words having " << word[index] << " at position " << index << "\n";
                removeStringsAtPosition(_availableWords.at(wip.back()), word[index], index);
                //std::cout << " # strings after removal: " << _availableWords.at(wip.back()).size() << "\n";

            } else {
				std::cout << "schema unfeasible. exiting.\n";
				break;
			}
            continue;
        }

        if (_grid.done()) {
            break;
        }
        // now get the next slot, which is the one with the fewest possibilities
        Slot* next = nullptr;
        int nextBest = 0;
        for (const auto& a : _availableWords) {
            if (!_grid.isComplete(a.first) && (next == nullptr || a.second.size() < nextBest)) {
                next = a.first;
                nextBest = a.second.size();
            }
        }
        std::cout << " next is: " << next->toString() << "\n";
        wip.push_back(next);
        _grid.dump();
    }
	_grid.dump();
        //this->dumnp();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "terminated in " << duration.count() << " msec.\n";
    _grid.dump();
}

//void Decision::undo() {
//    if (!_currentWord.empty()) {
//        _grid.clearWord(_slot);
//        _currentWord = "";
//    }
//}

//
//void DecisionTree::pop(std::shared_ptr<Decision> d) {
//    auto parent = d->getParent();
//    const auto& ch = parent->getChildren();
//    std::list<std::shared_ptr<Decision>> nodesToRemove(ch.begin(), ch.end());
//    while (!nodesToRemove.empty()) {
//        auto c = nodesToRemove.front();
//        c->undo();
//        nodesToRemove.pop_front();
//        _nodes.erase(c->slot());
//        _leaves.remove(c);
//        nodesToRemove.insert(nodesToRemove.end(), c->getChildren().begin(), c->getChildren().end());
//    }
//    parent->clearChildren();
//    // parent becomes a leaf
//    _leaves.push_front(_nodes.at(parent->slot()));
//    // restore the original state
//    parent->undo();
//
//
//}
//
//void DecisionTree::dump() {
//    std::list<std::shared_ptr<Decision>> l{_root};
//    while (!l.empty()) {
//        auto current = l.front();
//        l.pop_front();
//        std::cout << current->slot()->toString() << " (" << current->getPossibilities() << "): " ;
//        for (auto& c : current->getChildren()) {
//
//            l.push_back(c);
//            std::cout << c->slot()->toString() << " (" << c->getPossibilities() << "), ";
//        }
//        std::cout << "\n";
//
//    }
//}