#pragma once

#include "Compiler.hpp"
#include "Diagnostic.hpp"
#include "Entity/Scope.hpp"
#include "Entity/Symbol.hpp"
#include "Entity/Type.hpp"
#include "Units/Package.hpp"
#include "Manager/this.hpp"
#include <vector>

struct SemanticAnalyzer {
  public:
    using TypeContext = MaybeUndefined<TypeInfo>;

    SemanticAnalyzer(PackageId package_id, Compiler &compiler,
                     DiagnosticPool &diagnostics);
    auto Run(this SemanticAnalyzer &) -> void;
    auto GetPackage(this SemanticAnalyzer &) -> Package *;
    auto GetPackage(this SemanticAnalyzer const &) -> Package const *;

    auto FindSymbol(this SemanticAnalyzer const &, std::string_view)
        -> MaybeUndefined<SymbolId>;
    auto FindSymbol(this SemanticAnalyzer &, std::string_view)
        -> MaybeUndefined<SymbolId>;

    auto FindSymbolEagerly(this SemanticAnalyzer const &, std::string_view)
        -> Symbol const *;
    auto FindSymbolEagerly(this SemanticAnalyzer &, std::string_view)
        -> Symbol *;

    auto FindSymbolIn(this SemanticAnalyzer const &, ScopeId, std::string_view)
        -> MaybeUndefined<SymbolId>;
    auto FindSymbolIn(this SemanticAnalyzer &, ScopeId, std::string_view)
        -> MaybeUndefined<SymbolId>;

    auto GetSymbolIn(this SemanticAnalyzer const &, ScopeId, std::string_view)
        -> Symbol const *;
    auto GetSymbolIn(this SemanticAnalyzer &, ScopeId, std::string_view)
        -> Symbol *;

    auto FindSymbolEagerlyIn(this SemanticAnalyzer const &, ScopeId,
                             std::string_view) -> Symbol const *;
    auto FindSymbolEagerlyIn(this SemanticAnalyzer &, ScopeId, std::string_view)
        -> MaybeUndefined<SymbolId>;

    auto RegisterSymbol(this SemanticAnalyzer &, ScopeId, SymbolRef)
        -> SymbolId;

    auto Report(this SemanticAnalyzer &, Diagnostic &&) -> void;

    auto GetCompiler(this SemanticAnalyzer const &) -> Compiler const &;
    auto GetCompiler(this SemanticAnalyzer &) -> Compiler &;

    auto GetCurrentScope(this SemanticAnalyzer const &) -> ScopeId;

    auto SetTypeContext(this SemanticAnalyzer &, SemanticAnalyzer::TypeContext) -> /* old */ SemanticAnalyzer::TypeContext;
    auto GetTypeContext(this SemanticAnalyzer const &) -> SemanticAnalyzer::TypeContext;
  private:
    PackageId m_packageId;
    Compiler &m_compiler;
    DiagnosticPool &m_diagnostics;
    TaskManager m_taskManager;

    ScopeId m_currentScope;

    SemanticAnalyzer::TypeContext m_typeContext;
};
