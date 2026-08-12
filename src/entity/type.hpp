#pragma once

#include "common/common.hpp"
#include "ast/type_hint.hpp"

struct TypeId {
    explicit TypeId(usize id) : id(id) {}
    usize const id;

    auto operator==(this TypeId one, TypeId two) -> bool {
        return one.id == two.id;
    }
};

struct TypeInfo {
    public:
        TypeId id;
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