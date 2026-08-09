#pragma once

#include "ast/ast.hpp"
#include "entity/symbol.hpp"
#include "lexer/source_location.hpp"

class Namespace : public Symbol {
  public:
    Namespace(std::string const &name, SourceLocation definition);

    auto get_ast(this Namespace &self) -> AstProgram & { return self.m_ast; }

  private:
    SymbolRef m_parent;
    AstProgram m_ast;
};
