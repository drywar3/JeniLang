#pragma once

#include "Ast/this.hpp"
#include "Entity/Symbol.hpp"
#include "Lexer/SourceLocation.hpp"

class Namespace : public Symbol {
    using parent_ref = std::optional<SymbolId>;
  public:
    Namespace(std::string const &name, SourceLocation definition_location, std::optional<AstId> id = std::nullopt);

    auto GetAstId(this Namespace const &self) -> AstId;

	static auto withAst(std::string const &name, SourceLocation definition_location, AstId id) -> Namespace;

  private:
    parent_ref m_parent;
    std::optional<AstId> m_astId;
};
