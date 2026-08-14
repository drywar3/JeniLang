#pragma once

#include "ast/defs.hpp"
#include "sema/manager/this.hpp"
#include "sema/this.hpp"

namespace sema {
    auto check_function(SemanticAnalyzer &sema, Task::Payload payload)
        -> TaskResult;
}