
#include "sema/checks/check_statement.hpp"
#include "ast/statement.hpp"
#include "sema/checks/check_compound_statement.hpp"
#include "sema/manager/this.hpp"
#include "sema/this.hpp"

auto sema::check_statement(SemanticAnalyzer &sema, Task::Payload payload)
    -> TaskResult {
    auto const *statement = std::get<Statement const *>(payload);
    switch (statement->get_kind()) {
    case StatementKind::CompoundStatement:
        return check_compound_statement(sema, payload);
    default:
        TODO();
    }
}