#include "ast/misc.hpp"
#include "ast/type_hint.hpp"
#include "common/common.hpp"
#include "lexer/source_location.hpp"
#include "lexer/token.hpp"
#include "parser/parse_impl.hpp"
#include "parser/parser.hpp"
#include "ast/defs.hpp"
#include <initializer_list>
#include <memory>
#include <optional>
#include <utility>

static constexpr std::initializer_list<TokenKind> ITEM_HEAD = {
    TokenKind::KwImport, TokenKind::KwUsing,     TokenKind::KwStruct,
    TokenKind::KwEnum,   TokenKind::KwNamespace, TokenKind::KwVar,
    TokenKind::KwConst,  TokenKind::KwComptime,  TokenKind::KwInterface,
    TokenKind::KwExtend, TokenKind::KwFunc,
};

auto parse_name(Parser &parser, Name &name) -> bool {
    if (!parser.is(TokenKind::Identifier)) {
        return false;
    }

    name.location = parser.current().location;
    name.name     = parser.slice(name.location);
    parser.next();
    return true;
}

static auto parse_function_declaration(Parser &parser, AttributeList attrs)
    -> std::optional<ItemRef>;

auto parse_item(Parser &parser) -> std::optional<ItemRef> {
    auto attrs = parse_attributes(parser);
    if (auto head = parser.eat_one_of(ITEM_HEAD); head.has_value()) {
        switch (head.value().kind) {
        case TokenKind::KwVar:
        case TokenKind::KwConst:
            return parse_variable_declaration(parser, std::move(attrs));
        case TokenKind::KwFunc:
            return parse_function_declaration(parser, std::move(attrs));
        default:
            TODO();
        }
    }

    return std::nullopt;
}

auto parse_variable_declaration(Parser &parser, AttributeList attrs)
    -> std::optional<std::unique_ptr<VariableDeclaration>> {
    auto mutability = parser.previous().kind == TokenKind::KwConst
                          ? Mutability::Immutable
                          : Mutability::Mutable;
    auto var = std::make_unique<VariableDeclaration>(parser.previous().location,
                                                     mutability, attrs);
    // Parse Name.
    if (!parse_name(parser, var->get_name()))
        PANIC("could not parse name");
    // Parse Type hint
    if (parser.try_eat(TokenKind::SpColon)) {
        if (auto type_hint = parse_type_hint(parser); type_hint.has_value()) {
            var->get_type_hint() = std::move(type_hint.value());
        } else {
            if (!parser.skip_to(TokenKind::OpAssign))
                return std::nullopt;
        }
    } else {
        var->get_type_hint() = std::nullopt;
    }

    // Parse Initializer
    if (parser.try_eat(TokenKind::OpAssign)) {
        var->get_initializer() = parse_expression(parser);
    }

    parser.eat(TokenKind::SpSemicolon);

    return var;
}

static auto parse_function_prototype(Parser &parser, FunctionPrototype &prototype) 
    -> bool {
    if (!parser.try_eat(TokenKind::SpOpenParen)) PANIC("expected `(` function parameter list");

    while (!parser.is(TokenKind::SpCloseParen)) {
        FunctionParameter parameter;

        parameter.policy = FunctionParameterPolicy::Copy;
        if (parser.try_eat(TokenKind::KwMove)) parameter.policy = FunctionParameterPolicy::Move;

        if (!parse_name(parser, parameter.name)) PANIC("expected parameter name");

        // After the parameter name, we could have two cases
        // `:=` - type_hint is skipped and default value is provided
        // `:`  - type_hint is provided

        if (parser.try_eat(TokenKind::SpColon) and !parser.is(TokenKind::OpAssign)) {
            if (auto type_hint = parse_type_hint(parser); type_hint.has_value())
                parameter.type_hint = std::move(type_hint.value());
            else PANIC("expected parameter type after `:`");
        }

        if (parser.try_eat(TokenKind::OpAssign)) {
            if (auto initializer = parse_expression(parser); initializer.has_value()) {
                parameter.default_value = std::move(initializer.value());
            } else PANIC();
        }

        prototype.parameters.push_back(std::move(parameter));

        if (!parser.try_eat(TokenKind::SpComma)) break;
    }

    if (!parser.try_eat(TokenKind::SpCloseParen)) return false;

    if (parser.try_eat(TokenKind::OpArrow)) {
        prototype.return_type = std::move(parse_type_hint(parser).value());
    } else {
        prototype.return_type = std::make_unique<TypeHint>(SourceLocation::NULLLOC(), Mutability::Immutable, TypeHintKind::Void);
    }

    return true;
}

auto parse_function_declaration(Parser &parser, AttributeList attrs)
    -> std::optional<ItemRef> {
    auto beginning = parser.previous().location;
    Name function_name{};
    FunctionPrototype function_prototype{};
    FunctionDeclaration::Effects function_effects{};
    std::optional<StatementRef> function_body;

    if (!parse_name(parser, function_name)) PANIC();
    if (!parse_function_prototype(parser, function_prototype)) PANIC();

    if (parser.is(TokenKind::SpOpenBrace)) {
        function_body = TRY(parse_statement(parser));
    } else if (parser.try_eat(TokenKind::SpSemicolon)) {
        function_body = std::nullopt;
    } else PANIC();

    return std::make_unique<FunctionDeclaration>(
        beginning.merge(parser.previous().location),
        attrs,
        function_name,
        function_prototype,
        function_effects,
        function_body
    );
}
