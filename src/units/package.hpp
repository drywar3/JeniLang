#pragma once

#include <string>
#include <optional>
#include "entity/scope.hpp"
#include "common/idmap.hpp"
#include "common/common.hpp"
#include "entity/namespace.hpp"

struct StorageId {
    public:
        explicit StorageId(usize id) : id(id) {}
        usize const id;
};

static constexpr ScopeId GLOBAL_SCOPE(0);

struct Package {
  public:
    Package(std::string const &name, SourceId of_root_file);

    auto get_root(this Package const &) -> SymbolRef const *;
    auto get_root(this Package &) -> SymbolRef *;

    auto get_root_id(this Package const &) -> SymbolId;
    auto get_root_source_file(this Package const &) -> SourceId;
	auto get_ast(this Package &, AstId) -> Ast &;

    auto get_scope(this Package const &, ScopeId) -> Scope const &;
    auto get_scope(this Package &, ScopeId) -> Scope &;

  private:
    std::string m_name;
    SymbolId m_root;
    SourceId m_root_source_file;
    std::vector<Scope> m_scopes;
	std::vector<Ast> m_asts;
};

struct PackageId {
    usize id;
    explicit PackageId(usize id) : id(id) {}
};
