#include "entity/namespace.hpp"
#include "ast/ast.hpp"
#include <cassert>

Namespace::Namespace(std::string const &name, SourceLocation definition, std::optional<AstId> id)
    : Symbol(std::move(name), definition), m_ast_id(id) {}


auto Namespace::with_ast(std::string const &name, SourceLocation definition_location, AstId id) -> Namespace {
    return Namespace(name, definition_location, id);
}

auto Namespace::get_ast_id(this Namespace const &self) -> AstId {
    assert(self.m_ast_id.has_value());
    return self.m_ast_id.value();
}
