#include "ast/statement.hpp"
#include "lexer/token.hpp"
#include "parser/parse_impl.hpp"
#include "parser/parser.hpp"
#include <initializer_list>

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

auto parse_statement(Parser &parser) -> std::optional<StatementRef> {
    if (auto head = parser.try_eat_one_of(STMT_HEADS); head.has_value()) {
        switch (head.value().kind) {
            case TokenKind::KwVar:
            case TokenKind::KwConst:
                return parse_variable_declaration(parser, {});
            default:
                TODO();
        }
    }

    return parse_expression(parser);
}
