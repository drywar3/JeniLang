#pragma once

#include "Ast/Item.hpp"
#include "Lexer/SourceLocation.hpp"
#include <memory>

enum struct StatementKind {
    VariableDeclaration,
    UsingStatement,
    ForLoop,
    WhileLoop,
    IfStatement,
    Return,
    Break,
    Continue,
    CompoundStatement,
    Expression,
};

struct Statement {
  public:
    Statement(SourceLocation location, StatementKind kind)
        : m_location(location), m_kind(kind) {}
    virtual ~Statement() = default;

    auto GetLocation(this Statement const &) -> SourceLocation;
    auto GetKind() const -> StatementKind { return m_kind; }

  private:
    SourceLocation m_location;
    StatementKind m_kind;
};

using StatementRef = std::unique_ptr<Statement>;

