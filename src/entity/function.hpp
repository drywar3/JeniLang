#pragma once

#include "entity/symbol.hpp"
#include "entity/type.hpp"
#include "ast/expression.hpp"
#include "lexer/source_location.hpp"
#include <vector>

namespace concrete
{

    struct ParameterInfo {
        // Name of the parameter
        std::string name;
        // the associated type information.
        // left as `nullptr` before type checking.
        MaybeUndefined<TypeInfo> type;
        // the associated default expression to be
        // substituted incase of omission.
        ExpressionRef default_expression;
    };

    struct FunctionPrototype {
      public:
        std::vector<ParameterInfo> parameters;
        TypeInfo return_type;

      private:
    };
} // namespace concrete

struct FunctionSymbol : public Symbol {
  public:
    FunctionSymbol(std::string &&name, SourceLocation source_location);

    auto prototype_is_defined(this FunctionSymbol const &) -> bool;
    
    auto get_prototype(this FunctionSymbol const &) -> concrete::FunctionPrototype const*;
    auto get_prototype(this FunctionSymbol &) -> concrete::FunctionPrototype*;

    auto set_prototype(this FunctionSymbol &, concrete::FunctionPrototype &) -> void;
  private:
    std::string m_linkage_name;
    MaybeUndefined<concrete::FunctionPrototype> m_prototype;
};
