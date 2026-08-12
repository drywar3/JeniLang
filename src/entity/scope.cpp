#include "scope.hpp"

Scope::Scope(Scope::parent_ref parent, Kind kind)
    : m_parent(parent), m_kind(kind), m_storage() {

}

auto Scope::put(this Scope &self, std::string const& name, SymbolRef symbol) -> SymbolId {
    return SymbolId(self.m_storage.replace_or_insert(name, std::move(symbol)));
}

auto Scope::get_from_name(this Scope const &self, std::string_view name) -> SymbolRef const * {
    return self.m_storage.get(std::string(name));
}

auto Scope::get_from_name(this Scope &self, std::string_view name) -> SymbolRef * {
    return self.m_storage.get(std::string(name));
}

auto Scope::get_from_id(this Scope &self, SymbolId id) -> SymbolRef * {
    return self.m_storage.get_from_id_unchecked(id.id);
}
