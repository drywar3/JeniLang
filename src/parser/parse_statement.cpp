#include "ast/statement.hpp"
#include "lexer/token.hpp"
#include "parser/parse_impl.hpp"
#include "parser/parser.hpp"
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <optional>

static constexpr std::initializer_list<TokenKind> STMT_HEADS = {
    TokenKind::KwVar,
    TokenKind::KwConst,
    TokenKind::KwComptime,
    TokenKind::KwContinue,
    TokenKind::KwBreak,
    TokenKind::KwLabel,
    TokenKind::KwLoop,
    TokenKind::KwFor,
    TokenKind::KwIf,
    TokenKind::KwWhile,
    TokenKind::KwDrop,
    TokenKind::KwPanic,
};

auto parse_compound_statement(Parser &parser)
    -> std::optional<StatementRef> {
    assert(parser.previous().kind == TokenKind::SpOpenBrace);
    while (!parser.is(TokenKind::SpCloseBrace)) {

    }
    parser.try_eat(TokenKind::SpCloseBrace);
    return std::nullopt;
}

auto parse_statement(Parser &parser) -> std::optional<StatementRef> {
    if (auto head = parser.try_eat_one_of(STMT_HEADS); head.has_value()) {
        switch (head.value().kind) {
            case TokenKind::KwVar:
            case TokenKind::KwConst:
                return parse_variable_declaration(parser, {});
            case TokenKind::SpOpenBrace:
                return parse_compound_statement(parser);
            default:
                TODO();
        }
    }

    return parse_expression(parser);
}
