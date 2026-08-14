#pragma once

#include "Ast/TypeHint.hpp"
#include "Entity/Symbol.hpp"
#include "Entity/Type.hpp"
#include "Sema/this.hpp"


constexpr TypeId TYPEID_VOID(0);
constexpr TypeId TYPEID_ERROR(1);
constexpr TypeId TYPEID_INT32(2);
constexpr TypeId TYPEID_BASE(3);


namespace sema {

    struct ResolveResult {
        public:
            explicit ResolveResult();
            ResolveResult(TypeInfo info) : m_info(info) {}
            ResolveResult(SymbolId dependency) : m_dependency(dependency) {}

            static auto Undefined() -> ResolveResult;

            auto IsUnResolved(this ResolveResult const &) -> bool;
            auto IsUndefined(this ResolveResult const &) -> bool;
            auto IsDefinedAndResolved(this ResolveResult const &) -> bool;

            auto GetInfo(this ResolveResult const &) -> TypeInfo;
            auto GetDependency(this ResolveResult const &) -> SymbolId;
        private:
            MaybeUndefined<SymbolId> m_dependency;
            TypeInfo m_info;
    };

    struct TypeResolver {
        public:
            // Given a syntax-type, resolve and produces a concrete semantic-type
            // if successful, produces the TypeInfo of the typehint and returns null
            // else if no such type exists, produces TYPEID_ERROR and returns null
            // else if type exists but is unresolved, returns TYPEID_ERROR and returns it's symbol id.
            static auto Resolve(SemanticAnalyzer &, TypeHint const *)
                -> ResolveResult;
    };
}