#include "Parser/ParseImpl.hpp"
#include "Ast/this.hpp"
#include "Entity/Namespace.hpp"
#include "Parser/this.hpp"

auto ParseIntoNamespace(Parser &parser, Namespace &out) -> void {
    Ast &ast = parser.m_GetCC()
                   ->GetPackage(parser.GetPackage())
                   .value()
                   ->GetAst(out.GetAstId());
    while (!parser.IsDone()) {
        if (auto item = ParseItem(parser); item.has_value()) {
            ast.push_back(std::move(item.value()));
        }
    }
}
