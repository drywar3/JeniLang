#pragma once

#include "sema/this.hpp"

namespace sema {
    auto run_semantic_checks(SemanticAnalyzer &sema) -> bool;
}