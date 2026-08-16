#include "CheckFunction.hpp"
#include "Ast/Defs.hpp"
#include "Ast/Item.hpp"
#include "Common/this.hpp"
#include "Entity/Function.hpp"
#include "Entity/Type.hpp"
#include "Sema/Checks/CheckCompoundStatement.hpp"
#include "Sema/Checks/CheckFunction.hpp"
#include "Sema/Manager/this.hpp"
#include "Sema/TypeChecker/this.hpp"
#include "Sema/this.hpp"
#include "Units/Package.hpp"
#include <cassert>

static auto CheckFunctionPrototype(SemanticAnalyzer &sema,
                                     Task::Payload payload) -> TaskResult {
    Item const *item = std::get<Item const *>(payload);
    FunctionDeclaration const *function =
        static_cast<FunctionDeclaration const *>(item);

    FunctionSymbol *symbol = static_cast<FunctionSymbol *>(
        sema.GetSymbolIn(GLOBAL_SCOPE, function->GetName().name));
    if (symbol->PrototypeIsDefined())
        return TaskResult::Completed;

    concrete::FunctionPrototype prototype{};

    for (auto &parameter : function->GetPrototype().parameters) {
        concrete::ParameterInfo parameterInfo;

        parameterInfo.name = parameter.name.name;
    
        if (sema::ResolveResult result = sema::TypeResolver::Resolve(sema, parameter.typeHint->get()); result.IsDefinedAndResolved()) {
            parameterInfo.type = result.GetInfo();
        } else if (result.IsUnResolved()) {
            TODO();
        } else if (result.IsUndefined()) {
            TODO();
        }

        prototype.parameters.push_back(std::move(parameterInfo));
    }

    if (sema::ResolveResult result = sema::TypeResolver::Resolve(sema, function->GetPrototype().returnType.get());
        result.IsDefinedAndResolved()) 
            prototype.return_type = result.GetInfo();
    else if (result.IsUnResolved())
        return TaskResult::Waiting;
    else TODO("error");
    
    symbol->SetPrototype(prototype);
    return TaskResult::Completed;
}

auto sema::CheckFunction(SemanticAnalyzer &sema, Task::Payload payload)
    -> TaskResult {
    Item const *item = std::get<Item const *>(payload);
    FunctionDeclaration const *function =
        static_cast<FunctionDeclaration const *>(item);
    auto *symbol = sema.GetSymbolIn(GLOBAL_SCOPE, function->GetName().name);
    assert(symbol != nullptr);

    if (TaskResult x = CheckFunctionPrototype(sema, item);
        x == TaskResult::Waiting)
        return TaskResult::Waiting;

    if (auto function_body = function->GetBody()) {    
        if (CheckCompoundStatement(sema, function_body) == TaskResult::Waiting) 
            return TaskResult::Waiting;
    }

    return TaskResult::Completed;
}