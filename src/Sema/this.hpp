#pragma once

#include "Compiler.hpp"
#include "Diagnostic.hpp"
#include "Entity/Scope.hpp"
#include "Entity/Symbol.hpp"
#include "Units/Package.hpp"
#include "Manager/this.hpp"

struct SemanticAnalyzer {
  public:
    SemanticAnalyzer(PackageId package_id, Compiler &compiler,
                     DiagnosticPool &diagnostics);
    auto Run(this SemanticAnalyzer &) -> void;
    auto GetPackage(this SemanticAnalyzer &) -> Package *;
    auto GetPackage(this SemanticAnalyzer const &) -> Package const *;

    auto FindSymbol(this SemanticAnalyzer const &, std::string_view)
        -> Symbol const *;
    auto FindSymbol(this SemanticAnalyzer &, std::string_view) -> Symbol *;

    auto FindSymbolEagerly(this SemanticAnalyzer const &, std::string_view)
        -> Symbol const *;
    auto FindSymbolEagerly(this SemanticAnalyzer &, std::string_view)
        -> Symbol *;

    auto FindSymbolIn(this SemanticAnalyzer const &, ScopeId, std::string_view)
        -> Symbol const *;
    auto FindSymbolIn(this SemanticAnalyzer &, ScopeId, std::string_view)
        -> Symbol *;

    auto FindSymbolEagerlyIn(this SemanticAnalyzer const &, ScopeId,
                             std::string_view) -> Symbol const *;
    auto FindSymbolEagerlyIn(this SemanticAnalyzer &, ScopeId, std::string_view)
        -> Symbol *;

    auto RegisterSymbol(this SemanticAnalyzer &, ScopeId, SymbolRef)
        -> SymbolId;


    auto Report(this SemanticAnalyzer &, Diagnostic &&) -> void;
  private:
    PackageId m_packageId;
    Compiler &m_compiler;
    DiagnosticPool &m_diagnostics;
    TaskManager m_taskManager;

    ScopeId m_currentScope;
};
