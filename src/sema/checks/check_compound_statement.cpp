#include "check_compound_statement.hpp"
#include "ast/defs.hpp"
#include "ast/statement.hpp"
#include "sema/checks/check_compound_statement.hpp"
#include "sema/checks/check_statement.hpp"
#include "sema/manager/this.hpp"
#include "sema/this.hpp"

auto sema::check_compound_statement(SemanticAnalyzer &sema,
                                    Task::Payload payload) -> TaskResult {
    auto const *statement = std::get<Statement const *>(payload);
    auto const *compound_statement =
        static_cast<CompoundStatement const *>(statement);

    for (auto const &statement : compound_statement->get_body()) {
        if (auto result = check_statement(sema, statement.get());
            result == TaskResult::Waiting) {
            return TaskResult::Waiting;
        }
    }
    return TaskResult::Completed;
}