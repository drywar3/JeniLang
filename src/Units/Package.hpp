#pragma once

#include <string>
#include <optional>
#include "Entity/Scope.hpp"
#include "Common/IdMap.hpp"
#include "Common/this.hpp"
#include "Entity/Namespace.hpp"
#include "Entity/Symbol.hpp"

struct StorageId {
    public:
        explicit StorageId(usize id) : id(id) {}
        usize const id;
};

static constexpr ScopeId GLOBAL_SCOPE(0);

struct Package {
  public:
    Package(std::string const &name, SourceId ofRootFile);

    auto GetRoot(this Package const &) -> Symbol const *;
    auto GetRoot(this Package &) -> Symbol *;

    auto GetRootId(this Package const &) -> SymbolId;
    auto GetRootSourceFile(this Package const &) -> SourceId;
	auto GetAst(this Package &, AstId) -> Ast &;

    auto GetScope(this Package const &, ScopeId) -> Scope const &;
    auto GetScope(this Package &, ScopeId) -> Scope &;

  private:
    std::string m_name;
    SymbolId m_root;
    SourceId m_rootSourceFile;
    std::vector<Scope> m_scopes;
	std::vector<Ast> m_asts;
};

struct PackageId {
    usize id;
    explicit PackageId(usize id) : id(id) {}
};
