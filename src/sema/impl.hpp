#pragma once

#include "this.hpp"

namespace sema {
    auto register_item(SemanticAnalyzer &sema, ItemRef const &item) -> bool;
    auto register_function(SemanticAnalyzer &sema, ItemRef const& item) -> bool;
}
