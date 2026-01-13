#pragma once

#include "grid.h"
#include "dict.h"
#include <list>

//class Decision {
//
//public:
//    Decision(Slot* slot, Grid& grid, Dictionary& dict);
//    int chooseWord();
//    void addChild(std::shared_ptr<Decision>);
//    Slot* slot() {
//        return _slot;
//    }
//    Decision* getParent();
//    std::vector<std::shared_ptr<Decision>>& getChildren() {
//        return _children;
//    }
//    void clearChildren() {
//        _children.clear();
//    }
//
//    std::unordered_map<Slot*, std::vector<std::string>> getInterections() {
//        return _ints;
//    }
//    void setParent(Decision*);
//    std::string getBaseState() const;
//    void undo();
//    int getPossibilities() const;
//    std::string getChoice() const {
//        return _currentWord;
//    }
//    const std::unordered_set<std::shared_ptr<Slot>>& getIntersections() const {
//        return _intersections;
//    }
//    void findValidWords();
//private:
//    Grid& _grid;
//    Dictionary& _dict;
//    Slot* _slot;
//    // the available words for this slot
//    std::vector<std::string> words;
//    std::vector<std::string> valid;
//    std::unordered_set<std::shared_ptr<Slot>> _intersections;
//    std::string _currentWord;
//    std::vector<std::shared_ptr<Decision>> _children;
//    Decision* _parent;
//    std::unordered_map<Slot*, std::vector<std::string>> _ints;
//    std::string _currentState;
//};
//
//inline int Decision::getPossibilities() const {
//    return words.size();
//}
//
//inline std::string Decision::getBaseState() const {
//    return _currentState;
//}
//inline void Decision::setParent(Decision * d) {
//    _parent = d;
//}
//
//inline Decision * Decision::getParent() {
//    return _parent;
//
//}
class DecisionTree {
public:
    DecisionTree(Grid& grid, Dictionary& dict);
    // we should try to get the decision with the least words!
    //Decision* getNext();
    //void add(std::shared_ptr<Decision> decision, std::shared_ptr<Decision> parent);
    int process();
    void dump();
    void removeStringsAtPosition(std::vector<std::string>& vec, char c, size_t n);

private:
    Grid& _grid;
    Dictionary& _dict;
    //void pop(std::shared_ptr<Decision>);
    //std::unordered_map<Slot*, std::shared_ptr<Decision>> _nodes;
    //std::list<std::shared_ptr<Decision>> _leaves;
    //std::shared_ptr<Decision> _root;
    std::unordered_map<Slot*, std::vector<std::string>> _availableWords;

};

