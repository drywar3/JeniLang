#pragma once

#include "common/idmap.hpp"
#include "entity/namespace.hpp"
#include "units/source_file.hpp"
#include "common/common.hpp"
#include <optional>
#include <string>

class Package {
  public:
    Package(std::string const &name, SourceId of_root_file);

    auto get_root(this Package const &self) -> SymbolId;
    auto get_root_source_file(this Package const &self) -> SourceId;
    auto get_symbol(this Package const &self, SymbolId id)
        -> std::optional<SymbolRef const &>;
    auto get_symbol(this Package &self, SymbolId id)
        -> std::optional<SymbolRef &>;

  private:
    std::string m_name;
    SymbolId m_root;
    SourceId m_root_source_file;
    IdMap<std::string, SymbolRef> m_symbols;
};

struct PackageId {
    usize id;
    explicit PackageId(usize id) : id(id) {}
};
