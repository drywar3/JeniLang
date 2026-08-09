#include "units/package.hpp"
#include "entity/namespace.hpp"
#include "lexer/source_location.hpp"
#include "units/source_file.hpp"
#include <memory>
#include <string>

Package::Package(std::string const &name, SourceId of_root_file)
    : m_name(name), m_root(0), m_root_source_file(of_root_file), m_symbols() {
    SymbolRef root_namespace = std::make_unique<Namespace>(
        "root", SourceLocation::beginning(of_root_file));
    m_root.id = m_symbols.insert("root", std::move(root_namespace));
}

auto Package::get_root(this Package const &self) -> SymbolId {
    return self.m_root;
}

auto Package::get_symbol(this Package const &self, SymbolId id)
    -> std::optional<SymbolRef const &> {
    return self.m_symbols.get_from_id(id.id);
}

auto Package::get_symbol(this Package &self, SymbolId id)
    -> std::optional<SymbolRef &> {
    return self.m_symbols.get_from_id(id.id);
}

auto Package::get_root_source_file(this Package const &self) -> SourceId {
    return self.m_root_source_file;
}
