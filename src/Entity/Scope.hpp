#pragma once

#include "Common/this.hpp"
#include "Common/IdMap.hpp"
#include "Entity/Symbol.hpp"

struct ScopeId {
    public:
        constexpr explicit ScopeId(usize id) : id(id) {}
        usize id;

        auto operator==(this ScopeId one, ScopeId two) -> bool {
            return one.id == two.id;
        }
};

using SymbolStorage = IdMap<std::string, SymbolRef>;

struct Scope {
    public:
        using Parent = std::optional<ScopeId>;
        enum struct Kind {
            Function,
            Block,
            Loop,
            Global,
        };

        Scope(Parent parent, Kind kind);

        auto Put(this Scope &, std::string const &, SymbolRef symbol) -> SymbolId;

        auto GetFromName(this Scope const &, std::string_view) -> MaybeUndefined<SymbolId>;

        auto GetFromId(this Scope &, SymbolId) -> Symbol *;
        auto GetFromId(this Scope const &, SymbolId) -> Symbol const *;

        auto GetParent(this Scope const &self) -> Scope::Parent 
        {
            return self.m_parent;
        }

    private:
        Parent m_parent;
        Kind    m_kind;
        SymbolStorage m_storage;
};


