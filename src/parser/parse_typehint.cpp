#include "ast/type_hint.hpp"
#include "lexer/token.hpp"
#include "parser/parser.hpp"
#include "common/common.hpp"
#include <memory>
#include <optional>

// [const | var]
auto parse_type_hint(Parser &parser) -> std::optional<TypeHintRef> {
    auto begin = parser.current().location;

    Mutability mutability = parser.try_eat(TokenKind::KwConst)
                                ? Mutability::Immutable
                                : Mutability::Mutable;
    if (mutability == Mutability::Mutable)
        parser.try_eat(TokenKind::KwVar);

    // Pointer
    if (parser.try_eat(TokenKind::OpStar)) {
        if (auto inner = parse_type_hint(parser); inner.has_value()) {
            return std::make_unique<Pointer>(
                begin.merge(inner.value()->get_location()), mutability,
                std::move(inner.value()));
        } else
            return std::nullopt;
    }

    // Reference
    if (parser.try_eat(TokenKind::OpAmp)) {
        TODO("parse reference type");
    }

    // Array: [<expression>]<type>
    // Slice: []<type>
    if (parser.try_eat(TokenKind::SpOpenBracket)) {
        TODO("parse array type");
    }

    auto loc = begin.merge(parser.current().location);

    switch (parser.next().kind) {
    case TokenKind::TkUint:
    case TokenKind::TkUint32:
        return std::make_unique<UnsignedInteger>(loc, mutability,
                                                 IntegerWidth::Int32);
    case TokenKind::TkInt:
    case TokenKind::TkInt32:
        return std::make_unique<SignedInteger>(loc, mutability,
                                               IntegerWidth::Int32);
    default:
        break;
    }

    UNREACHABLE();
}
