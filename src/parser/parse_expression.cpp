#include "ast/expression.hpp"
#include "lexer/token.hpp"
#include "parser/parser.hpp"
#include "common/common.hpp"
#include <cstdlib>
#include <optional>
#include <string_view>

using ParseResult = std::optional<ExpressionRef>;

auto parse_primary(Parser &parser) -> ParseResult {
    if (parser.try_eat(TokenKind::IntegerLiteral)) {
        std::string_view token = parser.slice(parser.previous().location);
        u64 int_value          = std::strtoull(token.data(), nullptr, 10);
        return std::make_unique<IntegerLiteral>(parser.previous().location,
                                                int_value);
    }
    PANIC("(%s)", to_string(parser.current().kind).data());
}

auto parse_postfix(Parser &parser) -> ParseResult {
    return parse_primary(parser);
}

auto parse_unary(Parser &parser) -> ParseResult {
    return parse_postfix(parser);
}

auto parse_factor(Parser &parser) -> ParseResult { return parse_unary(parser); }

auto parse_term(Parser &parser) -> ParseResult { return parse_factor(parser); }

auto parse_relational(Parser &parser) -> ParseResult {
    return parse_term(parser);
}

auto parse_conditional(Parser &parser) -> ParseResult {
    return parse_relational(parser);
}

auto parse_expression(Parser &parser) -> std::optional<ExpressionRef> {
    return parse_conditional(parser);
}
