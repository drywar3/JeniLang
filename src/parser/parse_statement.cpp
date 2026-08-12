#include "ast/defs.hpp"
#include "ast/statement.hpp"
#include "lexer/token.hpp"
#include "parser/parse_impl.hpp"
#include "parser/parser.hpp"
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <memory>
#include <optional>
#include <vector>

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
    -> std::optional<std::unique_ptr<CompoundStatement>> {
    assert(parser.previous().kind == TokenKind::SpOpenBrace);
    std::vector<StatementRef> stmts{};
    auto beginning = parser.previous().location;
    while (!parser.is(TokenKind::SpCloseBrace)) {
        if (auto statement = parse_statement(parser); statement.has_value()) {
            stmts.push_back(std::move(statement.value()));
        } else PANIC();
    }
    parser.try_eat(TokenKind::SpCloseBrace);
    return std::make_unique<CompoundStatement>(beginning, parser.previous().location, stmts);
}

auto parse_statement(Parser &parser) -> std::optional<StatementRef> {
	if (parser.try_eat(TokenKind::SpOpenBrace)) return parse_compound_statement(parser);

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
