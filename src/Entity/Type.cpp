#include "Entity/Type.hpp"
#include "Common/this.hpp"
#include "Sema/TypeChecker/this.hpp"
#include <ranges>

static auto StructureTypeMatch(StructureType const &one,
                               StructureType const &two) -> bool
{
    if (one.GetName() != two.GetName())
        return false;
    if (one.GetFields().size() != two.GetFields().size())
        return false;

    for (auto const &[field1, field2] :
         std::ranges::zip_view(one.GetFields(), two.GetFields())) {
        if (field1.name != field2.name)
            return false;
        if (field1.type.id != field2.type.id)
            return false;
        if (field1.type.mutability != field2.type.mutability)
            return false;
    }

    return true;
}

TypeRegistry::TypeRegistry() {}

auto TypeRegistry::RegisterType(this TypeRegistry &self, TypeData data)
    -> TypeId
{
    TypeId id{self.m_typeData.size()};
    self.m_typeData.push_back(data);
    return id;
}

auto TypeRegistry::FindOrRegisterType(this TypeRegistry &self, TypeData data)
    -> TypeId
{
    usize id = 0;
    for (TypeData const &_data : self.m_typeData) {
        if (_data.GetKind() == data.GetKind()) {
            if (_data.GetKind() == TypeKind::Structure) {
                if (StructureTypeMatch(_data.GetStruct(), data.GetStruct())) {
                    return TypeId(id);
                }
            } else
                return TypeId(id);
        }
        id += 1;
    }
    return self.RegisterType(data);
}

TypeData::TypeData(TypeKind kind, TypePayload payload)
    : m_kind(kind), m_payload(std::move(payload))
{
}

auto TypeData::GetKind(this TypeData const &self) -> TypeKind
{
    return self.m_kind;
}

auto TypeData::GetSize(this TypeData const &self) -> usize
{
    return self.m_size;
}

auto TypeData::GetStruct(this TypeData const &self) -> StructureType const &
{
    JENI_COMPILER_ASSERT(self.GetKind() == TypeKind::Structure,
                         "Invalid Structure Type Access");
    return std::get<StructureType>(self.m_payload);
}

auto StructureType::GetName(this StructureType const &self) -> std::string_view
{
    return self.m_name.subview();
}

auto StructureType::GetFields(this StructureType const &self)
    -> std::vector<StructureType::Field> const &
{
    return self.m_fields;
}

auto TypeRegistry::InitializeBuiltinTypes(this TypeRegistry &self) -> void
{

    static TypeData data[] = {[TYPEID_VOID.id]  = TypeData(TypeKind::Void),
                              [TYPEID_INT32.id] = TypeData(TypeKind::Int32)};
    self.m_typeData.append_range(data);

    JENI_COMPILER_ASSERT(self.m_typeData[TYPEID_VOID.id].GetKind() == TypeKind::Void,
                        "Builin Type Registration Failed");
}

auto TypeId::IntoInfo(this TypeId id, Mutability mutability) -> TypeInfo
{
    return TypeInfo(id, mutability);
}