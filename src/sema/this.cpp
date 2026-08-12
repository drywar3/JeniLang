#include "this.hpp"
#include "units/package.hpp"

SemanticAnalyzer::SemanticAnalyzer(PackageId package_id, Compiler &compiler, DiagnosticPool &diagnostics) 
    : m_package_id(package_id), m_compiler(compiler), m_diagnostics(diagnostics), m_current_scope(GLOBAL_SCOPE) {
}

auto SemanticAnalyzer::get_package(this SemanticAnalyzer &self) -> Package * {
    return self.m_compiler.get_package(self.m_package_id).value();; 
}

auto SemanticAnalyzer::get_package(this SemanticAnalyzer const &self) -> Package const* {
    return self.m_compiler.get_package(self.m_package_id).value();; 
}


auto SemanticAnalyzer::find_symbol(this SemanticAnalyzer const &self, std::string_view name) -> SymbolRef const * {
    Scope const &scope = self.get_package()->get_scope(self.m_current_scope);
    return scope.get_from_name(name);
}

auto SemanticAnalyzer::find_symbol(this SemanticAnalyzer &self, std::string_view name) -> SymbolRef * { 
    Scope &scope = self.get_package()->get_scope(self.m_current_scope);
    return scope.get_from_name(name);
}

auto SemanticAnalyzer::find_symbol_eagerly(this SemanticAnalyzer const &, std::string_view) -> SymbolRef const * { TODO(); }
auto SemanticAnalyzer::find_symbol_eagerly(this SemanticAnalyzer &, std::string_view) -> SymbolRef* { TODO(); }
auto SemanticAnalyzer::find_symbol_in(this SemanticAnalyzer const &, ScopeId, std::string_view) -> SymbolRef const * { TODO(); }

auto SemanticAnalyzer::find_symbol_in(this SemanticAnalyzer &self, ScopeId scope_id, std::string_view name) -> SymbolRef * { 
    auto *package = self.get_package();
    auto &scope = package->get_scope(scope_id);
    return scope.get_from_name(name);
}

auto SemanticAnalyzer::find_symbol_eagerly_in(this SemanticAnalyzer const &, ScopeId, std::string_view) -> SymbolRef const * { TODO(); }
auto SemanticAnalyzer::find_symbol_eagerly_in(this SemanticAnalyzer &, ScopeId, std::string_view) -> SymbolRef* { TODO(); }

auto SemanticAnalyzer::register_symbol(this SemanticAnalyzer &self, ScopeId scope_id, SymbolRef symbol) -> SymbolId {
    auto *package = self.get_package();
    auto &scope = package->get_scope(scope_id);
    if (auto _ = scope.get_from_name(symbol->get_name())) {
        if (scope_id != GLOBAL_SCOPE)  
            PANIC("attempt to redefine symbol in non-global scope");
    }
    return scope.put(std::string(symbol->get_name()), std::move(symbol));
}