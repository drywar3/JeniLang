#include "Sema/Checks/CheckExpression.hpp"
#include "Ast/Expression.hpp"
#include "Sema/Manager/this.hpp"
#include "Sema/this.hpp"

auto CheckInteger(SemanticAnalyzer &sema, Task::Payload payload) -> TaskResult
{
    TODO();
}

auto sema::CheckExpression(SemanticAnalyzer &sema, Task::Payload payload)
    -> TaskResult
{
    Expression const *expression = std::get<Expression const *>(payload);
    switch (expression->GetKind()) {
    case ExpressionKind::IntegerLiteral:
        return CheckInteger(sema,
                            static_cast<IntegerLiteral const *>(expression));
    default:
        TODO();
    }
}