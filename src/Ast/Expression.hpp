#pragma once

#include "Ast/Statement.hpp"
#include "Ast/TypeHint.hpp"
#include "Lexer/SourceLocation.hpp"
#include "Common/this.hpp"
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

    auto GetLocation(this Expression const &self) -> SourceLocation {
        return self.m_location;
    }
    auto GetKind() const -> ExpressionKind { return m_kind; }

  private:
    SourceLocation m_location;
    ExpressionKind m_kind;
};

using ExpressionRef = std::unique_ptr<Expression>;

struct IntegerLiteral : public Expression {
    IntegerLiteral(SourceLocation location, u64 value, bool is_signed = true,
                   IntegerWidth width = IntegerWidth::Int32)
        : Expression(location, ExpressionKind::IntegerLiteral), value(value),
          isSigned(is_signed), width(width) {}

    u64 value;
    bool isSigned;
    IntegerWidth width;
};
