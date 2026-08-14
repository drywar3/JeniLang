#pragma once

#include "ast/type_hint.hpp"
#include "entity/symbol.hpp"
#include "entity/type.hpp"
#include "sema/this.hpp"


constexpr TypeId TYPEID_VOID(0);
constexpr TypeId TYPEID_INT32(1);
constexpr TypeId TYPEID_BASE(2);


namespace sema {
    struct TypeResolver {
        public:
            struct Error {
                public:
                    // non-null indicates that resolution of a type is dependent on this symbol.
                    // null     indicates that resolution is invalid.
                    MaybeUndefined<SymbolId> dependency;
            };

            // Given a syntax-type, resolve and return a concrete semantic-type
            static auto resolve(SemanticAnalyzer &sema, TypeHint const *type_hint)
                -> MayError<TypeInfo, Error>;
    };
}