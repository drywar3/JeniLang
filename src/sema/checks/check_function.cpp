#include "check_function.hpp"
#include "ast/defs.hpp"
#include "ast/item.hpp"
#include "entity/function.hpp"
#include "entity/type.hpp"
#include "sema/checks/check_compound_statement.hpp"
#include "sema/checks/check_function.hpp"
#include "sema/manager/this.hpp"
#include "sema/this.hpp"
#include "sema/type_checker/this.hpp"
#include "units/package.hpp"
#include <algorithm>
#include <cassert>
#include <variant>

static auto check_function_prototype(SemanticAnalyzer &sema,
                                     Task::Payload payload) -> TaskResult {
    Item const *item = std::get<Item const *>(payload);
    FunctionDeclaration const *function =
        static_cast<FunctionDeclaration const *>(item);

    FunctionSymbol *symbol = static_cast<FunctionSymbol *>(
        sema.find_symbol_in(GLOBAL_SCOPE, function->get_name().name));
    if (symbol->prototype_is_defined())
        return TaskResult::Completed;

    concrete::FunctionPrototype prototype{};

    for (auto &parameter : function->get_prototype().parameters) {
        concrete::ParameterInfo parameter_info;

        parameter_info.name = parameter.name.name;

        if (auto x =
                sema::TypeResolver::resolve(sema, parameter.type_hint->get());
            x.has_value()) {
            parameter_info.type = x.value();
        } else if (auto dependency = x.error(); dependency.dependency) {
            TODO("Dependency on another symbol detected");
            return TaskResult::Waiting;
        } else {
            PANIC("undefined symbol detected");
        }

        prototype.parameters.push_back(std::move(parameter_info));
    }

    if (auto x = sema::TypeResolver::resolve(
            sema, function->get_prototype().return_type.get());
        x.has_value()) {
        prototype.return_type = x.value();
    } else if (x.error().dependency.has_value())
        return TaskResult::Waiting;
    else
        PANIC();

    symbol->set_prototype(prototype);
    return TaskResult::Completed;
}

auto sema::check_function(SemanticAnalyzer &sema, Task::Payload payload)
    -> TaskResult {
    Item const *item = std::get<Item const *>(payload);
    FunctionDeclaration const *function =
        static_cast<FunctionDeclaration const *>(item);
    auto *symbol = sema.find_symbol_in(GLOBAL_SCOPE, function->get_name().name);
    assert(symbol != nullptr);

    if (TaskResult x = check_function_prototype(sema, item);
        x == TaskResult::Waiting)
        return TaskResult::Waiting;

    if (auto function_body = function->get_body()) {    
        if (check_compound_statement(sema, function_body) == TaskResult::Waiting) 
            return TaskResult::Waiting;
    }

    return TaskResult::Completed;
}