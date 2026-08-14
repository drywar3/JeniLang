#include "Ast/Defs.hpp"
#include "Ast/Statement.hpp"
#include "Lexer/Token.hpp"
#include "Parser/ParseImpl.hpp"
#include "Parser/this.hpp"
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

auto ParseCompoundStatement(Parser &parser)
    -> std::optional<std::unique_ptr<CompoundStatement>> {
    assert(parser.Previous().kind == TokenKind::SpOpenBrace);
    std::vector<StatementRef> stmts{};
    auto beginning = parser.Previous().location;
    while (!parser.Is(TokenKind::SpCloseBrace)) {
        if (auto statement = ParseStatement(parser); statement.has_value()) {
            stmts.push_back(std::move(statement.value()));
        } else PANIC();
    }
    parser.TryEat(TokenKind::SpCloseBrace);
    return std::make_unique<CompoundStatement>(beginning, parser.Previous().location, stmts);
}

auto ParseStatement(Parser &parser) -> std::optional<StatementRef> {
	if (parser.TryEat(TokenKind::SpOpenBrace)) return ParseCompoundStatement(parser);

    if (auto head = parser.TryEatOneOf(STMT_HEADS); head.has_value()) {
        switch (head.value().kind) {
            case TokenKind::KwVar:
            case TokenKind::KwConst:
                return ParseVariableDeclaration(parser, {});
            case TokenKind::SpOpenBrace:
                return ParseCompoundStatement(parser);
            default:
                TODO();
        }
    }

    return ParseExpression(parser);
}
