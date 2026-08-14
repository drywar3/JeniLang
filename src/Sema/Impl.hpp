#pragma once

#include "this.hpp"

namespace sema {
    auto RegisterItem(SemanticAnalyzer &sema, ItemRef const &item) -> bool;
    auto RegisterFunction(SemanticAnalyzer &sema, ItemRef const& item) -> bool;
}
