#pragma once

#include "Ast/Defs.hpp"
#include "Sema/Manager/this.hpp"
#include "Sema/this.hpp"

namespace sema {
    auto CheckFunction(SemanticAnalyzer &sema, Task::Payload payload)
        -> TaskResult;
}