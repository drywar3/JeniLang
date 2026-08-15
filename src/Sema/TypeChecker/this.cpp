#include "this.hpp"
#include "Ast/TypeHint.hpp"
#include "Common/this.hpp"
#include "Diagnostic.hpp"
#include "Entity/Symbol.hpp"
#include "Entity/Type.hpp"
#include "Sema/this.hpp"
#include <optional>

auto sema::TypeResolver::Resolve(SemanticAnalyzer &sema,
                                 TypeHint const *typeHint) -> ResolveResult {
    auto const kind       = typeHint->GetKind();
    auto const mutability = typeHint->GetMutability();
    switch (kind) {
    case TypeHintKind::Pointer: {
        auto const *pointer = static_cast<Pointer const *>(typeHint);
        auto innerInfo      = Resolve(sema, pointer->GetInner());
        if (innerInfo.IsDefinedAndResolved()) {
            return sema.GetCompiler().GetRegistry().FindOrRegisterType(
                TypeData(TypeKind::Pointer, PointerType(innerInfo.GetInfo())))
                .IntoInfo(mutability);
        } else TODO();
    } break;
    case TypeHintKind::SignedInteger: {
        auto const *signed_integer =
            static_cast<SignedInteger const *>(typeHint);
        switch (signed_integer->width) {
        case IntegerWidth::Int32:
            return ResolveResult(TypeInfo(TYPEID_INT32, mutability));
        default:
            TODO();
        }
    } break;
    case TypeHintKind::Void:
        return ResolveResult(TypeInfo(TYPEID_VOID, mutability));
    default: {
        // No such type.
        sema.Report(Diagnostic(Severity::Error, "no such type", "here",
                               typeHint->GetLocation()));
        return ResolveResult::Undefined();
    };
    }
}

sema::ResolveResult::ResolveResult() : m_dependency(), m_info(TYPEID_ERROR) {}

auto sema::ResolveResult::Undefined() -> ResolveResult {
    return ResolveResult();
}

auto sema::ResolveResult::IsDefinedAndResolved(
    this sema::ResolveResult const &self) -> bool {
    return self.m_info.id != TYPEID_ERROR and !self.m_dependency.has_value();
}

auto sema::ResolveResult::IsUndefined(this sema::ResolveResult const &self)
    -> bool {
    return self.m_info.id == TYPEID_ERROR and !self.m_dependency.has_value();
}

auto sema::ResolveResult::IsUnResolved(this sema::ResolveResult const &self)
    -> bool {
    return self.m_info.id == TYPEID_ERROR and self.m_dependency.has_value();
}

auto sema::ResolveResult::GetDependency(this sema::ResolveResult const &self)
    -> SymbolId {
    JENI_COMPILER_ASSERT(self.IsUnResolved(),
                         "type resolution has no dependency");
    return self.m_dependency.value();
}

auto sema::ResolveResult::GetInfo(this sema::ResolveResult const &self)
    -> TypeInfo {
    JENI_COMPILER_ASSERT(self.IsDefinedAndResolved(),
                         "type resolution did not yield type info");
    return self.m_info;
}