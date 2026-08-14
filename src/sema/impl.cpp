#include "common/common.hpp"
#include "entity/function.hpp"
#include "sema/checks/this.hpp"
#include "this.hpp"
#include "impl.hpp"
#include "ast/defs.hpp"
#include "units/package.hpp"
#include <memory>
#include <utility>

auto run_discovery_phase(SemanticAnalyzer &sema) -> bool {
    Package *package = sema.get_package();
    Ast &root_ast    = package->get_ast(AstId(0));

    for (ItemRef const &item : root_ast) {
        sema::register_item(sema, item);
    }

    return true;
}

auto SemanticAnalyzer::run(this SemanticAnalyzer &self) -> void {
    run_discovery_phase(self);
    sema::run_semantic_checks(self);
}

auto sema::register_item(SemanticAnalyzer &sema, ItemRef const &item) -> bool {
    switch (item->get_kind()) {
    case ItemKind::FunctionDeclaration:
        return register_function(sema, item);
    default:
        TODO();
    }
}

auto sema::register_function(SemanticAnalyzer &sema, ItemRef const &item)
    -> bool {
    FunctionDeclaration const *function =
        static_cast<FunctionDeclaration const *>(item.get());
    if (auto symbol =
            sema.find_symbol_in(GLOBAL_SCOPE, function->get_name().name)) {
        PANIC("redeclaration of symbol: %s", symbol->get_name().data());
    }
    Box<FunctionSymbol> symbol = std::make_unique<FunctionSymbol>(
        std::string(function->get_name().name), function->get_name().location);
    auto id = sema.register_symbol(GLOBAL_SCOPE, std::move(symbol));
    return true;
}
