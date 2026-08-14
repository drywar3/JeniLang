#include "Ast/TypeHint.hpp"
#include "Lexer/Token.hpp"
#include "Parser/this.hpp"
#include "Common/this.hpp"
#include <memory>
#include <optional>

// [const | var]
auto ParseTypeHint(Parser &parser) -> std::optional<TypeHintRef> {
    auto begin = parser.Current().location;

    Mutability mutability = parser.TryEat(TokenKind::KwConst)
                                ? Mutability::Immutable
                                : Mutability::Mutable;
    if (mutability == Mutability::Mutable)
        parser.TryEat(TokenKind::KwVar);

    // Pointer
    if (parser.TryEat(TokenKind::OpStar)) {
        if (auto inner = ParseTypeHint(parser); inner.has_value()) {
            return std::make_unique<Pointer>(
                begin.Merge(inner.value()->GetLocation()), mutability,
                std::move(inner.value()));
        } else
            return std::nullopt;
    }

    // Reference
    if (parser.TryEat(TokenKind::OpAmp)) {
        TODO("parse reference type");
    }

    // Array: [<expression>]<type>
    // Slice: []<type>
    if (parser.TryEat(TokenKind::SpOpenBracket)) {
        TODO("parse array type");
    }

    auto loc = begin.Merge(parser.Current().location);

    switch (parser.Next().kind) {
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
