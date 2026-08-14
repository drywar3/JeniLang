
#include "Sema/Checks/CheckStatement.hpp"
#include "Ast/Statement.hpp"
#include "Sema/Checks/CheckCompoundStatement.hpp"
#include "Sema/Manager/this.hpp"
#include "Sema/this.hpp"

auto sema::CheckStatement(SemanticAnalyzer &sema, Task::Payload payload)
    -> TaskResult {
    auto const *statement = std::get<Statement const *>(payload);
    switch (statement->GetKind()) {
    case StatementKind::CompoundStatement:
        return CheckCompoundStatement(sema, payload);
    default:
        TODO();
    }
}