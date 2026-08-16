#pragma once

#include "Entity/Symbol.hpp"
#include "Entity/Type.hpp"
#include "Lexer/SourceLocation.hpp"

enum struct VariableState {
    Live,
    Dead,
    PartiallyDead,
};

struct VariableSymbol : public Symbol {
  public:
    VariableSymbol(std::string &&, SourceLocation, MaybeUndefined<TypeInfo>,
                   VariableState state = VariableState::Live);

    auto GetTypeInfo(this VariableSymbol const &) -> MaybeUndefined<TypeInfo>;
    auto GetTypeInfo(this VariableSymbol &) -> TypeInfo *;

    auto IsTypeDefined(this VariableSymbol const &) -> bool;

    auto GetState(this VariableSymbol const &) -> VariableState;
    auto SetState(this VariableSymbol &, VariableState) -> void;

  private:
    MaybeUndefined<TypeInfo> m_typeInfo;
    VariableState m_state;
};