#include "Common/this.hpp"
#include "Entity/Function.hpp"
#include "Sema/Checks/this.hpp"
#include "this.hpp"
#include "Impl.hpp"
#include "Ast/Defs.hpp"
#include "Units/Package.hpp"
#include <memory>
#include <utility>

auto run_discovery_phase(SemanticAnalyzer &sema) -> bool {
    Package *package = sema.GetPackage();
    Ast &root_ast    = package->GetAst(AstId(0));

    for (ItemRef const &item : root_ast) {
        sema::RegisterItem(sema, item);
    }

    return true;
}

auto SemanticAnalyzer::Run(this SemanticAnalyzer &self) -> void {
    run_discovery_phase(self);
    sema::RunSemanticChecks(self);
}

auto sema::RegisterItem(SemanticAnalyzer &sema, ItemRef const &item) -> bool {
    switch (item->GetKind()) {
    case ItemKind::FunctionDeclaration:
        return RegisterFunction(sema, item);
    default:
        TODO();
    }
}

auto sema::RegisterFunction(SemanticAnalyzer &sema, ItemRef const &item)
    -> bool {
    FunctionDeclaration const *function =
        static_cast<FunctionDeclaration const *>(item.get());
    if (auto symbol =
            sema.GetSymbolIn(GLOBAL_SCOPE, function->GetName().name)) {
        PANIC("redeclaration of symbol: %s", symbol->GetName().data());
    }
    Box<FunctionSymbol> symbol = std::make_unique<FunctionSymbol>(
        std::string(function->GetName().name), function->GetName().location);
    auto id = sema.RegisterSymbol(GLOBAL_SCOPE, std::move(symbol));
    return true;
}
