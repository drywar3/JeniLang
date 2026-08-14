#include "Ast/TypeHint.hpp"
#include "Lexer/SourceLocation.hpp"

TypeHint::TypeHint(SourceLocation source_location, Mutability mutability,
                   TypeHintKind kind)
    : m_mutability(mutability), m_kind(kind), m_location(source_location) {}
