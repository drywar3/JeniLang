#include "Ast/Misc.hpp"
#include "Ast/TypeHint.hpp"
#include "Common/this.hpp"
#include "Lexer/SourceLocation.hpp"
#include "Lexer/Token.hpp"
#include "Parser/ParseImpl.hpp"
#include "Parser/this.hpp"
#include "Ast/Defs.hpp"
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

auto ParseName(Parser &parser, Name &name) -> bool {
    if (!parser.Is(TokenKind::Identifier)) {
        return false;
    }

    name.location = parser.Current().location;
    name.name     = parser.Slice(name.location);
    parser.Next();
    return true;
}

static auto ParseFunctionDeclaration(Parser &parser, AttributeList attrs)
    -> std::optional<ItemRef>;

auto ParseItem(Parser &parser) -> std::optional<ItemRef> {
    auto attrs = ParseAttributes(parser);
    if (auto head = parser.EatOneOf(ITEM_HEAD); head.has_value()) {
        switch (head.value().kind) {
        case TokenKind::KwVar:
        case TokenKind::KwConst:
            return ParseVariableDeclaration(parser, std::move(attrs));
        case TokenKind::KwFunc:
            return ParseFunctionDeclaration(parser, std::move(attrs));
        default:
            TODO();
        }
    }

    return std::nullopt;
}

auto ParseVariableDeclaration(Parser &parser, AttributeList attrs)
    -> std::optional<std::unique_ptr<VariableDeclaration>> {
    auto mutability = parser.Previous().kind == TokenKind::KwConst
                          ? Mutability::Immutable
                          : Mutability::Mutable;
    auto var = std::make_unique<VariableDeclaration>(parser.Previous().location,
                                                     mutability, attrs);
    // Parse Name.
    if (!ParseName(parser, var->get_name()))
        PANIC("could not parse name");
    // Parse Type hint
    if (parser.TryEat(TokenKind::SpColon)) {
        if (auto typeHint = ParseTypeHint(parser); typeHint.has_value()) {
            var->GetTypeHint() = std::move(typeHint.value());
        } else {
            if (!parser.SkipTo(TokenKind::OpAssign))
                return std::nullopt;
        }
    } else {
        var->GetTypeHint() = std::nullopt;
    }

    // Parse Initializer
    if (parser.TryEat(TokenKind::OpAssign)) {
        var->GetInitializer() = ParseExpression(parser);
    }

    parser.Eat(TokenKind::SpSemicolon);

    return var;
}

static auto ParseFunctionPrototype(Parser &parser, FunctionPrototype &prototype)
    -> bool {
    if (!parser.TryEat(TokenKind::SpOpenParen)) PANIC("expected `(` function parameter list");

    while (!parser.Is(TokenKind::SpCloseParen)) {
        FunctionParameter parameter;

        parameter.policy = FunctionParameterPolicy::Copy;
        if (parser.TryEat(TokenKind::KwMove)) parameter.policy = FunctionParameterPolicy::Move;

        if (!ParseName(parser, parameter.name)) PANIC("expected parameter name");

        // After the parameter name, we could have two cases
        // `:=` - type_hint is skipped and default value is provided
        // `:`  - type_hint is provided

        if (parser.TryEat(TokenKind::SpColon) and !parser.Is(TokenKind::OpAssign)) {
            if (auto typeHint = ParseTypeHint(parser); typeHint.has_value())
                parameter.typeHint = std::move(typeHint.value());
            else PANIC("expected parameter type after `:`");
        }

        if (parser.TryEat(TokenKind::OpAssign)) {
            if (auto initializer = ParseExpression(parser); initializer.has_value()) {
                parameter.defaultValue = std::move(initializer.value());
            } else PANIC();
        }

        prototype.parameters.push_back(std::move(parameter));

        if (!parser.TryEat(TokenKind::SpComma)) break;
    }

    if (!parser.TryEat(TokenKind::SpCloseParen)) return false;

    if (parser.TryEat(TokenKind::OpArrow)) {
        prototype.returnType = std::move(ParseTypeHint(parser).value());
    } else {
        prototype.returnType = std::make_unique<TypeHint>(SourceLocation::NULLLOC(), Mutability::Immutable, TypeHintKind::Void);
    }

    return true;
}

auto ParseFunctionDeclaration(Parser &parser, AttributeList attrs)
    -> std::optional<ItemRef> {
    auto beginning = parser.Previous().location;
    Name functionName{};
    FunctionPrototype functionPrototype{};
    FunctionDeclaration::Effects functionEffects{};
    std::optional<StatementRef> functionBody;

    if (!ParseName(parser, functionName)) PANIC();
    if (!ParseFunctionPrototype(parser, functionPrototype)) PANIC();

    if (parser.Is(TokenKind::SpOpenBrace)) {
        functionBody = TRY(ParseStatement(parser));
    } else if (parser.TryEat(TokenKind::SpSemicolon)) {
        functionBody = std::nullopt;
    } else PANIC();

    return std::make_unique<FunctionDeclaration>(
        beginning.Merge(parser.Previous().location),
        attrs,
        functionName,
        functionPrototype,
        functionEffects,
        functionBody
    );
}
