#include "ast/expression.hpp"
#include "ast/statement.hpp"
#include "lexer/source_location.hpp"

Expression::Expression(SourceLocation location, ExpressionKind kind)
    : Statement(location, StatementKind::Expression), m_location(location), m_kind(kind) {}
