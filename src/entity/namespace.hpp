#pragma once

#include "ast/ast.hpp"
#include "entity/symbol.hpp"
#include "lexer/source_location.hpp"

class Namespace : public Symbol {
    using parent_ref = std::optional<SymbolId>;
  public:
    Namespace(std::string const &name, SourceLocation definition_location, std::optional<AstId> id = std::nullopt);

    auto get_ast_id(this Namespace const &self) -> AstId;

	static auto with_ast(std::string const &name, SourceLocation definition_location, AstId id) -> Namespace;

  private:
    parent_ref m_parent;
    std::optional<AstId> m_ast_id;
};
