#include "Entity/Variable.hpp"
#include "Entity/Symbol.hpp"
#include "Entity/Type.hpp"
#include "Lexer/SourceLocation.hpp"
#include <string>

VariableSymbol::VariableSymbol(std::string &&name,
                               SourceLocation definitionLocation,
                               MaybeUndefined<TypeInfo> typeInfo,
                               VariableState state)
    : Symbol(name, definitionLocation), m_typeInfo(typeInfo), m_state(state)
{
}

auto VariableSymbol::GetTypeInfo(this VariableSymbol const &self)
    -> MaybeUndefined<TypeInfo>
{
    return self.m_typeInfo;
}

auto VariableSymbol::GetTypeInfo(this VariableSymbol &self)
    -> TypeInfo *
{
    if (self.m_typeInfo.has_value()) {
        return &self.m_typeInfo.value();
    }

    return nullptr;
}

auto VariableSymbol::IsTypeDefined(this VariableSymbol const &self)
    -> bool 
{
    return self.GetTypeInfo().has_value();
}

auto VariableSymbol::GetState(this VariableSymbol const &self)
    -> VariableState
{
    return self.m_state;
}

auto VariableSymbol::SetState(this VariableSymbol &self, VariableState state)
    -> void
{
    self.m_state = state;
}