#include "Ast/Expression.hpp"
#include "Ast/Statement.hpp"
#include "Lexer/SourceLocation.hpp"

Expression::Expression(SourceLocation location, ExpressionKind kind)
    : Statement(location, StatementKind::Expression), m_location(location), m_kind(kind) {}
