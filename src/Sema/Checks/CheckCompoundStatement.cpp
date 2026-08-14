#include "Ast/Defs.hpp"
#include "Ast/Statement.hpp"
#include "Sema/Checks/CheckCompoundStatement.hpp"
#include "Sema/Checks/CheckStatement.hpp"
#include "Sema/Manager/this.hpp"
#include "Sema/this.hpp"

auto sema::CheckCompoundStatement(SemanticAnalyzer &sema,
                                    Task::Payload payload) -> TaskResult {
    auto const *statement = std::get<Statement const *>(payload);
    auto const *compound_statement =
        static_cast<CompoundStatement const *>(statement);

    for (auto const &statement : compound_statement->GetBody()) {
        if (auto result = CheckStatement(sema, statement.get());
            result == TaskResult::Waiting) {
            return TaskResult::Waiting;
        }
    }
    return TaskResult::Completed;
}