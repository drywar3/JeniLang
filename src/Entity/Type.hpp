#pragma once

#include "Ast/Expression.hpp"
#include "Common/this.hpp"
#include "Ast/TypeHint.hpp"
#include "Entity/Symbol.hpp"
#include <string>
#include <string_view>
#include <variant>
#include <vector>

struct TypeInfo;

struct TypeId {
    constexpr TypeId(usize id) : id(id) {}
    usize id;

    auto operator==(this TypeId one, TypeId two) -> bool {
        return one.id == two.id;
    }

    auto IntoInfo(this TypeId, Mutability) -> TypeInfo;
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
    Int8,
    Int16,
    Int32,
    Int64,
    Uint8,
    Uint16,
    Uint32,
    Uint64,
    Usize,
    Isize,
    Char,
    Void,
    Bool,
    String,
    Pointer,
    Structure,
};

struct StructureType {
    public:
        struct Field {
            std::string name;
            TypeInfo    type;
            Expression const *defaultExpression;
        };        

        auto GetName(this StructureType const &) -> std::string_view;
        auto GetFields(this StructureType const &) -> std::vector<StructureType::Field> const &;
    private:
        std::string m_name;
        std::vector<StructureType::Field> m_fields;
};

struct PointerType {
    TypeInfo inner;
};

struct ReferenceType {
    TypeInfo inner;
};

using TypePayload = std::variant<std::monostate, ReferenceType, PointerType, StructureType>;

struct TypeData {
    public:
        TypeData() : m_kind(TypeKind::Void), m_size(0) {}
        TypeData(TypeKind kind, TypePayload payload = std::monostate());

        auto GetKind(this TypeData const &) -> TypeKind;
        auto GetSize(this TypeData const &) -> usize;

        auto GetStruct(this TypeData const &) -> StructureType const &;
    private:
        TypeKind m_kind;
        usize    m_size;

        TypePayload m_payload;
};

struct TypeRegistry {
    public:
        TypeRegistry();

        auto InitializeBuiltinTypes(this TypeRegistry &) -> void;
        
        auto RegisterType(this TypeRegistry &, TypeData) -> TypeId;
        auto FindOrRegisterType(this TypeRegistry &, TypeData) -> TypeId;

        auto GetType(this TypeRegistry &, TypeId) -> TypeData *;
        auto GetType(this TypeRegistry const &, TypeId) -> TypeData *;
    private:
        std::vector<TypeData> m_typeData;
};