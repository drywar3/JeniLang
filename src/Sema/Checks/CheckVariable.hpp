#pragma once

#include "Sema/Manager/this.hpp"
#include "Sema/this.hpp"

namespace sema {
    auto CheckVariableDeclaration(SemanticAnalyzer &, Task::Payload payload)
        -> TaskResult;
}