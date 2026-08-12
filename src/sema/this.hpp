#pragma once

#include "compiler.hpp"
#include "entity/scope.hpp"
#include "entity/symbol.hpp"
#include "units/package.hpp"
#include "manager/this.hpp"

struct SemanticAnalyzer {
    public:
        SemanticAnalyzer(PackageId package_id, Compiler &compiler, DiagnosticPool &diagnostics);
        auto run(this SemanticAnalyzer &) -> void;
        auto get_package(this SemanticAnalyzer &) -> Package *;
        auto get_package(this SemanticAnalyzer const &) -> Package const*;

        auto find_symbol(this SemanticAnalyzer const &, std::string_view) -> SymbolRef const *;
        auto find_symbol(this SemanticAnalyzer &, std::string_view) -> SymbolRef *;

        auto find_symbol_eagerly(this SemanticAnalyzer const &, std::string_view) -> SymbolRef const *;
        auto find_symbol_eagerly(this SemanticAnalyzer &, std::string_view) -> SymbolRef*;

        auto find_symbol_in(this SemanticAnalyzer const &, ScopeId, std::string_view) -> SymbolRef const *;
        auto find_symbol_in(this SemanticAnalyzer &, ScopeId, std::string_view) -> SymbolRef *;

        auto find_symbol_eagerly_in(this SemanticAnalyzer const &, ScopeId, std::string_view) -> SymbolRef const *;
        auto find_symbol_eagerly_in(this SemanticAnalyzer &, ScopeId, std::string_view) -> SymbolRef*;

auto register_symbol(this SemanticAnalyzer &, ScopeId, SymbolRef) -> SymbolId;
    private:
        PackageId m_package_id;
        Compiler &m_compiler;
        DiagnosticPool &m_diagnostics;
        TaskManager m_task_manager;

        ScopeId m_current_scope;
};
