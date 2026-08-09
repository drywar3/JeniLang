#include "ast/type_hint.hpp"
#include "lexer/source_location.hpp"

TypeHint::TypeHint(SourceLocation source_location, Mutability mutability,
                   TypeHintKind kind)
    : m_mutability(mutability), m_kind(kind), m_location(source_location) {}
