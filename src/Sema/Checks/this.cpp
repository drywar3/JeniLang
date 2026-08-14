#include "Sema/Checks/this.hpp"
#include "Ast/this.hpp"
#include "Ast/Defs.hpp"
#include "Ast/Item.hpp"
#include "Sema/Checks/CheckFunction.hpp"
#include "Sema/this.hpp"
#include "Units/Package.hpp"

auto sema::RunSemanticChecks(SemanticAnalyzer &sema) -> bool {
    Package *package = sema.GetPackage();
    Ast &ast         = package->GetAst(AstId(0));
    for (ItemRef const &item : ast) {
        if (item->GetKind() == ItemKind::FunctionDeclaration) {
            FunctionDeclaration const *function =
                static_cast<FunctionDeclaration const *>(item.get());
            sema::CheckFunction(sema, function);
        } else {
            TODO("diffent semantic symbol");
        }
    }
    return true;
}