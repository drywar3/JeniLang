#include "parser/parse_impl.hpp"
#include "ast/ast.hpp"
#include "entity/namespace.hpp"
#include "parser/parser.hpp"
#include <cstdio>
#include <sstream>

auto parse_into_namespace(Parser &parser, Namespace &out) -> void {
    Ast &ast = parser.m_get_cc()->get_package(parser.get_package()).value()
        ->get_ast(out.get_ast_id());
    while (!parser.is_done()) {
        if (auto item = parse_item(parser); item.has_value()) {
            std::stringstream ss;
            item.value()->to_string(ss);
            std::printf("%s\n", ss.str().c_str());
            ast.push_back(std::move(item.value()));
        }
    }
}
