#pragma once

#include "Sema/Manager/this.hpp"
#include "Sema/this.hpp"

namespace sema
{
    auto CheckExpression(SemanticAnalyzer &, Task::Payload payload)
        -> TaskResult;
}