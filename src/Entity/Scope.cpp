#include "Entity/Scope.hpp"
#include "Entity/Symbol.hpp"

Scope::Scope(Scope::Parent parent, Kind kind)
    : m_parent(parent), m_kind(kind), m_storage()
{
}

auto Scope::Put(this Scope &self, std::string const &name, SymbolRef symbol)
    -> SymbolId
{
    return SymbolId(self.m_storage.ReplaceOrInsert(name, std::move(symbol)));
}

auto Scope::GetFromName(this Scope const &self, std::string_view name)
    -> MaybeUndefined<SymbolId>
{
    if (auto id = self.m_storage.IdOf(std::string(name)); id.has_value()) {
        return SymbolId(id.value());
    }
    return std::nullopt;
}

auto Scope::GetFromId(this Scope &self, SymbolId id) -> Symbol *
{
    return self.m_storage.GetFromIdUnchecked(id.id)->get();
}

auto Scope::GetFromId(this Scope const &self, SymbolId id) -> Symbol const *
{
    return self.m_storage.GetFromIdUnchecked(id.id)->get();
}
