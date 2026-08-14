#pragma once

#include "Ast/Item.hpp"
#include "Ast/Misc.hpp"
#include "Ast/Statement.hpp"
#include "Ast/Expression.hpp"
#include "Ast/TypeHint.hpp"
#include "Parser/this.hpp"
#include "Ast/Defs.hpp"
#include <optional>

auto ParseItem(Parser &) -> std::optional<ItemRef>;
auto ParseStatement(Parser &) -> std::optional<StatementRef>;
auto ParseExpression(Parser &) -> std::optional<ExpressionRef>;
auto ParseTypeHint(Parser &) -> std::optional<TypeHintRef>;

auto ParseAttributes(Parser &) -> AttributeList;

auto FilterAttributeListBy(AttributeList const &, ItemKind);

auto ParseVariableDeclaration(Parser &, AttributeList)
    -> std::optional<std::unique_ptr<VariableDeclaration>>;
auto ParseCompoundStatement(Parser &)
	-> std::optional<std::unique_ptr<CompoundStatement>>;
