#include "Entity/Scope.hpp"

Scope::Scope(Scope::parent_ref parent, Kind kind)
    : m_parent(parent), m_kind(kind), m_storage() {

}

auto Scope::Put(this Scope &self, std::string const& name, SymbolRef symbol) -> SymbolId {
    return SymbolId(self.m_storage.ReplaceOrInsert(name, std::move(symbol)));
}

auto Scope::GetFromName(this Scope const &self, std::string_view name) -> SymbolRef const * {
    return self.m_storage.Get(std::string(name));
}

auto Scope::GetFromName(this Scope &self, std::string_view name) -> SymbolRef * {
    return self.m_storage.Get(std::string(name));
}

auto Scope::GetFromId(this Scope &self, SymbolId id) -> SymbolRef * {
    return self.m_storage.GetFromIdUnchecked(id.id);
}
