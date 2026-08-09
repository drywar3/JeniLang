#pragma once

#include "ast/item.hpp"
#include "lexer/source_location.hpp"
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
    Expression,
};

struct Statement {
  public:
    Statement(SourceLocation location, StatementKind kind)
        : m_location(location), m_kind(kind) {}
    virtual ~Statement() = default;

    auto get_location(this Statement const &) -> SourceLocation;
    auto get_kind() const -> StatementKind { return m_kind; }

  private:
    SourceLocation m_location;
    StatementKind m_kind;
};

using StatementRef = std::unique_ptr<Statement>;

