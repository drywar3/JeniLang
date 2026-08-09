#pragma once

#include "ast/statement.hpp"
#include "ast/type_hint.hpp"
#include "lexer/source_location.hpp"
#include "common/common.hpp"
#include <memory>

enum struct ExpressionKind {
    IntegerLiteral,
    FloatLiteral,
    StringLiteral,
    BooleanLiteral,
    AddressOf,
    Deference,
    Subscript,
    SliceOf,
    FunctionCall,
    CallableLiteral, // () {}
    Initializer,
    MemberAccess, // expr.expr | expr->expr
    Move,
    Cast,
};

struct Expression : public Statement {
  public:
    virtual ~Expression() = default;

    Expression(SourceLocation location, ExpressionKind kind);

    auto get_location(this Expression const &self) -> SourceLocation {
        return self.m_location;
    }
    auto get_kind() const -> ExpressionKind { return m_kind; }

  private:
    SourceLocation m_location;
    ExpressionKind m_kind;
};

using ExpressionRef = std::unique_ptr<Expression>;

struct IntegerLiteral : public Expression {
    IntegerLiteral(SourceLocation location, u64 value, bool is_signed = true,
                   IntegerWidth width = IntegerWidth::Int32)
        : Expression(location, ExpressionKind::IntegerLiteral), value(value),
          is_signed(is_signed), width(width) {}

    u64 value;
    bool is_signed;
    IntegerWidth width;
};
