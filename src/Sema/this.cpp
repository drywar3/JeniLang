#include "this.hpp"
#include "Diagnostic.hpp"
#include "Entity/Scope.hpp"
#include "Entity/Symbol.hpp"
#include "Entity/Type.hpp"
#include "Units/Package.hpp"
#include <optional>
#include <string_view>

SemanticAnalyzer::SemanticAnalyzer(PackageId package_id, Compiler &compiler,
                                   DiagnosticPool &diagnostics)
    : m_packageId(package_id), m_compiler(compiler), m_diagnostics(diagnostics),
      m_currentScope(GLOBAL_SCOPE)
{
}

auto SemanticAnalyzer::GetPackage(this SemanticAnalyzer &self) -> Package *
{
    return self.m_compiler.GetPackage(self.m_packageId).value();
    ;
}

auto SemanticAnalyzer::GetPackage(this SemanticAnalyzer const &self)
    -> Package const *
{
    return self.m_compiler.GetPackage(self.m_packageId).value();
    ;
}

auto SemanticAnalyzer::FindSymbol(this SemanticAnalyzer const &self,
                                  std::string_view name)
    -> MaybeUndefined<SymbolId>
{
    Scope const &scope = self.GetPackage()->GetScope(self.m_currentScope);
    return scope.GetFromName(name);
}

auto SemanticAnalyzer::FindSymbol(this SemanticAnalyzer &self,
                                  std::string_view name)
    -> MaybeUndefined<SymbolId>
{
    Scope &scope = self.GetPackage()->GetScope(self.m_currentScope);
    return scope.GetFromName(name);
}

auto SemanticAnalyzer::FindSymbolEagerly(this SemanticAnalyzer const &,
                                         std::string_view) -> Symbol const *
{
    TODO();
}
auto SemanticAnalyzer::FindSymbolEagerly(this SemanticAnalyzer &,
                                         std::string_view) -> Symbol *
{
    TODO();
}
auto SemanticAnalyzer::FindSymbolIn(this SemanticAnalyzer const &, ScopeId,
                                    std::string_view)
    -> MaybeUndefined<SymbolId>
{
    TODO();
}

auto SemanticAnalyzer::FindSymbolIn(this SemanticAnalyzer &self,
                                    ScopeId scopeId, std::string_view name)
    -> MaybeUndefined<SymbolId>
{
    Package *package = self.GetPackage();
    Scope &scope     = package->GetScope(scopeId);
    return scope.GetFromName(name);
}

auto SemanticAnalyzer::FindSymbolEagerlyIn(this SemanticAnalyzer const &,
                                           ScopeId, std::string_view)
    -> Symbol const *
{
    TODO();
}

auto SemanticAnalyzer::FindSymbolEagerlyIn(this SemanticAnalyzer &self,
                                           ScopeId scopeId,
                                           std::string_view name)
    -> MaybeUndefined<SymbolId>
{
    Package *package              = self.GetPackage();
    MaybeUndefined<ScopeId> scope = scopeId;
    while (scope.has_value()) {
        Scope &scopeObject = package->GetScope(scope.value());
        if (auto symbol = scopeObject.GetFromName(name)) {
            return symbol;
        }

        scope = scopeObject.GetParent();
    }
    return std::nullopt;
}

auto SemanticAnalyzer::RegisterSymbol(this SemanticAnalyzer &self,
                                      ScopeId scopeId, SymbolRef symbol)
    -> SymbolId
{
    auto *package = self.GetPackage();
    auto &scope   = package->GetScope(scopeId);
    if (auto _ = scope.GetFromName(symbol->GetName())) {
        if (scopeId != GLOBAL_SCOPE)
            PANIC("attempt to redefine symbol in non-global scope");
    }
    return scope.Put(std::string(symbol->GetName()), std::move(symbol));
}

auto SemanticAnalyzer::Report(this SemanticAnalyzer &self,
                              Diagnostic &&diagnostic) -> void
{
    self.m_diagnostics.Emit(std::move(diagnostic));
}

auto SemanticAnalyzer::GetCompiler(this SemanticAnalyzer const &self)
    -> Compiler const &
{
    return self.m_compiler;
}

auto SemanticAnalyzer::GetCompiler(this SemanticAnalyzer &self) -> Compiler &
{
    return self.m_compiler;
}

auto SemanticAnalyzer::GetCurrentScope(this SemanticAnalyzer const &self)
    -> ScopeId
{
    return self.m_currentScope;
}

auto SemanticAnalyzer::GetSymbolIn(this SemanticAnalyzer const &self,
                                   ScopeId scopeId, std::string_view name)
    -> Symbol const *
{
    if (auto id = self.FindSymbolIn(scopeId, name); id.has_value()) {
        return self.GetPackage()->GetScope(scopeId).GetFromId(id.value());
    }
    return nullptr;
}

auto SemanticAnalyzer::GetSymbolIn(this SemanticAnalyzer &self,
                                   ScopeId scopeId, std::string_view name)
    -> Symbol *
{
    if (auto id = self.FindSymbolIn(scopeId, name); id.has_value()) {
        return self.GetPackage()->GetScope(scopeId).GetFromId(id.value());
    }
    return nullptr;
}

auto SemanticAnalyzer::SetTypeContext(this SemanticAnalyzer &sema, SemanticAnalyzer::TypeContext info) -> MaybeUndefined<TypeInfo>
{
    auto old = sema.m_typeContext;
    sema.m_typeContext = info;
    return old;
}

auto SemanticAnalyzer::GetTypeContext(this SemanticAnalyzer const &self) -> MaybeUndefined<TypeInfo>
{
    return self.m_typeContext;
}