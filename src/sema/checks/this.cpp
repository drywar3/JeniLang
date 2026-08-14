#include "sema/checks/this.hpp"
#include "ast/ast.hpp"
#include "ast/defs.hpp"
#include "ast/item.hpp"
#include "sema/checks/check_function.hpp"
#include "sema/this.hpp"
#include "units/package.hpp"

auto sema::run_semantic_checks(SemanticAnalyzer &sema) -> bool {
    Package *package = sema.get_package();
    Ast &ast         = package->get_ast(AstId(0));
    for (ItemRef const &item : ast) {
        if (item->get_kind() == ItemKind::FunctionDeclaration) {
            FunctionDeclaration const *function =
                static_cast<FunctionDeclaration const *>(item.get());
            sema::check_function(sema, function);
        } else {
            TODO("diffent semantic symbol");
        }
    }
    return true;
}