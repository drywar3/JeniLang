#pragma once

#include "Lexer/SourceLocation.hpp"
#include <memory>

enum struct Mutability {
    Mutable,
    Immutable,
};

enum struct TypeHintKind {
    SignedInteger,
    UnsignedInteger,
    String,
    Boolean,
    Void,
    Pointer,
    Reference,
    Array,
    Slice,
};

struct TypeHint {
  public:
    TypeHint(SourceLocation location, Mutability mutability, TypeHintKind kind);
    virtual ~TypeHint() = default;

    auto GetKind(this TypeHint const &self) -> TypeHintKind {
        return self.m_kind;
    }
    auto GetLocation(this TypeHint const &self) -> SourceLocation {
        return self.m_location;
    }
    auto GetMutability(this TypeHint const &self) -> Mutability {
        return self.m_mutability;
    }

  private:
    Mutability m_mutability;
    TypeHintKind m_kind;
    SourceLocation m_location;
};

using TypeHintRef = std::unique_ptr<TypeHint>;

struct Pointer : public TypeHint {
  public:
    Pointer(SourceLocation location, Mutability mutability, TypeHintRef inner)
        : TypeHint(location, mutability, TypeHintKind::Pointer) {}

    auto GetInner(this Pointer const &self) -> TypeHintRef const & {
        return self.m_inner;
    }

  private:
    TypeHintRef m_inner;
};

enum struct IntegerWidth {
    Int8,
    Int16,
    Int32,
    Int64,
    Size,
};

struct SignedInteger : public TypeHint {
    SignedInteger(SourceLocation location, Mutability mutability,
                  IntegerWidth width)
        : TypeHint(location, mutability, TypeHintKind::SignedInteger),
          width(width) {}
    IntegerWidth width;
};

struct UnsignedInteger : public TypeHint {
    UnsignedInteger(SourceLocation location, Mutability mutability,
                    IntegerWidth width)
        : TypeHint(location, mutability, TypeHintKind::UnsignedInteger),
          width(width) {}
    IntegerWidth width;
};
