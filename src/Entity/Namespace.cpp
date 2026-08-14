#include "Entity/Namespace.hpp"
#include "Ast/this.hpp"
#include <cassert>

Namespace::Namespace(std::string const &name, SourceLocation definition, std::optional<AstId> id)
    : Symbol(std::move(name), definition), m_astId(id) {}


auto Namespace::withAst(std::string const &name, SourceLocation definition_location, AstId id) -> Namespace {
    return Namespace(name, definition_location, id);
}

auto Namespace::GetAstId(this Namespace const &self) -> AstId {
    assert(self.m_astId.has_value());
    return self.m_astId.value();
}
