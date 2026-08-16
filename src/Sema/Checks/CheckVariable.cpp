#include "Sema/Checks/CheckVariable.hpp"
#include "Ast/Defs.hpp"
#include "Ast/Item.hpp"
#include "Ast/Statement.hpp"
#include "Common/this.hpp"
#include "Entity/Scope.hpp"
#include "Entity/Symbol.hpp"
#include "Entity/Variable.hpp"
#include "Lexer/SourceLocation.hpp"
#include "Sema/Manager/this.hpp"
#include "Sema/TypeChecker/this.hpp"
#include "Sema/this.hpp"
#include "Units/Package.hpp"
#include <cassert>
#include <cstdio>
#include <memory>
#include <utility>
#include <variant>

auto sema::CheckVariableDeclaration(SemanticAnalyzer &sema,
                                    Task::Payload payload) -> TaskResult
{
    VariableDeclaration const *variable;
    if (std::holds_alternative<Item const *>(payload)) {
        variable = static_cast<VariableDeclaration const *>(
            std::get<Item const *>(payload));
    } else if (std::holds_alternative<Statement const *>(payload)) {
        variable = static_cast<VariableDeclaration const *>(
            std::get<Statement const *>(payload));
    } else
        JENI_COMPILER_ASSERT(false, "Unknown Construct");

    assert(variable);

    ScopeId currentScope = sema.GetCurrentScope();

    


    TODO();
}