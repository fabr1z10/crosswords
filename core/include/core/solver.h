#pragma once

#include "grid.h"
#include "dict.h"
#include <unordered_set>
#include <vector>



class Solver {
public:
    Solver() = default;

    void solve(Grid& g, Dictionary& dict);
};