#include "Ast/Expression.hpp"
#include "Lexer/Token.hpp"
#include "Parser/this.hpp"
#include "Common/this.hpp"
#include <cstdlib>
#include <optional>
#include <string_view>

using ParseResult = std::optional<ExpressionRef>;

auto ParsePrimary(Parser &parser) -> ParseResult {
    if (parser.TryEat(TokenKind::IntegerLiteral)) {
        std::string_view token = parser.Slice(parser.Previous().location);
        u64 intValue          = std::strtoull(token.data(), nullptr, 10);
        return std::make_unique<IntegerLiteral>(parser.Previous().location,
                                                intValue);
    }
    PANIC("(%s)", ToString(parser.Current().kind).data());
}

auto ParsePostfix(Parser &parser) -> ParseResult {
    return ParsePrimary(parser);
}

auto ParseUnary(Parser &parser) -> ParseResult {
    return ParsePostfix(parser);
}

auto ParseFactor(Parser &parser) -> ParseResult { return ParseUnary(parser); }

auto ParseTerm(Parser &parser) -> ParseResult { return ParseFactor(parser); }

auto ParseRelational(Parser &parser) -> ParseResult {
    return ParseTerm(parser);
}

auto ParseConditional(Parser &parser) -> ParseResult {
    return ParseRelational(parser);
}

auto ParseExpression(Parser &parser) -> std::optional<ExpressionRef> {
    return ParseConditional(parser);
}
