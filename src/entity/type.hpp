#pragma once

#include "common/common.hpp"
#include "ast/type_hint.hpp"

struct TypeId {
    constexpr TypeId(usize id) : id(id) {}
    usize id;

    auto operator==(this TypeId one, TypeId two) -> bool {
        return one.id == two.id;
    }
};

struct TypeInfo {
    public:
        TypeId id{0};
        Mutability mutability;

        auto operator==(this TypeInfo one,
                        TypeInfo two) -> bool {
            return one.id == two.id and one.mutability == two.mutability;
        }
    private:
};

enum struct TypeKind {
};

struct TypeLayout {
    public:
        virtual ~TypeLayout() = default;
    private:
};