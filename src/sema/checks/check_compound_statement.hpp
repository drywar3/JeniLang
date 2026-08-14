#pragma once

#include "sema/manager/this.hpp"
#include "sema/this.hpp"

namespace sema {
    auto check_compound_statement(SemanticAnalyzer &, Task::Payload payload)
        -> TaskResult;
}