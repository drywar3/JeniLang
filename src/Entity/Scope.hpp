#pragma once

#include "Common/this.hpp"
#include "Common/IdMap.hpp"
#include "Entity/Symbol.hpp"

struct ScopeId {
    public:
        constexpr explicit ScopeId(usize id) : id(id) {}
        usize const id;

        auto operator==(this ScopeId one, ScopeId two) -> bool {
            return one.id == two.id;
        }
};

using SymbolStorage = IdMap<std::string, SymbolRef>;

struct Scope {
    public:
        using parent_ref = std::optional<ScopeId>;
        enum struct Kind {
            Function,
            Block,
            Loop,
            Global,
        };

        Scope(parent_ref parent, Kind kind);

        auto Put(this Scope &, std::string const &, SymbolRef symbol) -> SymbolId;

        auto GetFromName(this Scope &, std::string_view) -> SymbolRef *;
        auto GetFromName(this Scope const &, std::string_view) -> SymbolRef const *;
        auto GetFromId(this Scope &, SymbolId) -> SymbolRef *;
    private:
        parent_ref m_parent;
        Kind    m_kind;
        SymbolStorage m_storage;
};


