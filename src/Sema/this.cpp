#include "this.hpp"
#include "Diagnostic.hpp"
#include "Units/Package.hpp"

SemanticAnalyzer::SemanticAnalyzer(PackageId package_id, Compiler &compiler,
                                   DiagnosticPool &diagnostics)
    : m_packageId(package_id), m_compiler(compiler), m_diagnostics(diagnostics),
      m_currentScope(GLOBAL_SCOPE) {}

auto SemanticAnalyzer::GetPackage(this SemanticAnalyzer &self) -> Package * {
    return self.m_compiler.GetPackage(self.m_packageId).value();
    ;
}

auto SemanticAnalyzer::GetPackage(this SemanticAnalyzer const &self)
    -> Package const * {
    return self.m_compiler.GetPackage(self.m_packageId).value();
    ;
}

auto SemanticAnalyzer::FindSymbol(this SemanticAnalyzer const &self,
                                  std::string_view name) -> Symbol const * {
    Scope const &scope = self.GetPackage()->GetScope(self.m_currentScope);
    return scope.GetFromName(name)->get();
}

auto SemanticAnalyzer::FindSymbol(this SemanticAnalyzer &self,
                                  std::string_view name) -> Symbol * {
    Scope &scope = self.GetPackage()->GetScope(self.m_currentScope);
    return scope.GetFromName(name)->get();
}

auto SemanticAnalyzer::FindSymbolEagerly(this SemanticAnalyzer const &,
                                         std::string_view) -> Symbol const * {
    TODO();
}
auto SemanticAnalyzer::FindSymbolEagerly(this SemanticAnalyzer &,
                                         std::string_view) -> Symbol * {
    TODO();
}
auto SemanticAnalyzer::FindSymbolIn(this SemanticAnalyzer const &, ScopeId,
                                    std::string_view) -> Symbol const * {
    TODO();
}

auto SemanticAnalyzer::FindSymbolIn(this SemanticAnalyzer &self,
                                    ScopeId scope_id, std::string_view name)
    -> Symbol * {
    auto *package = self.GetPackage();
    auto &scope   = package->GetScope(scope_id);
    if (auto x = scope.GetFromName(name)) {
        return x->get();
    }
    return nullptr;
}

auto SemanticAnalyzer::FindSymbolEagerlyIn(this SemanticAnalyzer const &,
                                           ScopeId, std::string_view)
    -> Symbol const * {
    TODO();
}
auto SemanticAnalyzer::FindSymbolEagerlyIn(this SemanticAnalyzer &, ScopeId,
                                           std::string_view) -> Symbol * {
    TODO();
}

auto SemanticAnalyzer::RegisterSymbol(this SemanticAnalyzer &self,
                                      ScopeId scope_id, SymbolRef symbol)
    -> SymbolId {
    auto *package = self.GetPackage();
    auto &scope   = package->GetScope(scope_id);
    if (auto _ = scope.GetFromName(symbol->GetName())) {
        if (scope_id != GLOBAL_SCOPE)
            PANIC("attempt to redefine symbol in non-global scope");
    }
    return scope.Put(std::string(symbol->GetName()), std::move(symbol));
}

auto SemanticAnalyzer::Report(this SemanticAnalyzer &self,
                              Diagnostic &&diagnostic) -> void {
    self.m_diagnostics.Emit(std::move(diagnostic));
}