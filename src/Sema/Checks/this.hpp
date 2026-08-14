#pragma once

#include "Sema/this.hpp"

namespace sema {
    auto RunSemanticChecks(SemanticAnalyzer &sema) -> bool;
}