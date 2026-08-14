#include "this.hpp"
#include "ast/type_hint.hpp"
#include "entity/symbol.hpp"
#include "entity/type.hpp"
#include "sema/this.hpp"

auto sema::TypeResolver::resolve(SemanticAnalyzer &sema, TypeHint const *type_hint) 
    -> MayError<TypeInfo, sema::TypeResolver::Error> {
    auto const kind = type_hint->get_kind();
    auto const mutability = type_hint->get_mutability();
    switch (kind) {
        case TypeHintKind::SignedInteger: {
            auto const *signed_integer =
                static_cast<SignedInteger const*>(type_hint);
            switch (signed_integer->width) {
            case IntegerWidth::Int32:
                return TypeInfo(TYPEID_INT32, mutability);
            default: TODO();
            }
        } break;
        case TypeHintKind::Void: return TypeInfo(TYPEID_VOID, mutability);
        default: TODO();
    }
}