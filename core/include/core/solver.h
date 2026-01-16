#pragma once

#include "grid.h"
#include "dict.h"
#include <unordered_set>
#include <vector>
#include "core/decision.h"


class Solver {
public:
    Solver() = default;

    void solve(Grid& g, Dictionary& dict, SeedMode mode, uint32_t seed = 0);
};