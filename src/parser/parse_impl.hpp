#pragma once

#include "ast/item.hpp"
#include "ast/misc.hpp"
#include "ast/statement.hpp"
#include "ast/expression.hpp"
#include "ast/type_hint.hpp"
#include "parser/parser.hpp"
#include "ast/defs.hpp"
#include <memory>
#include <optional>

auto parse_item(Parser &) -> std::optional<ItemRef>;
auto parse_statement(Parser &) -> std::optional<StatementRef>;
auto parse_expression(Parser &) -> std::optional<ExpressionRef>;
auto parse_type_hint(Parser &) -> std::optional<TypeHintRef>;

auto parse_attributes(Parser &) -> AttributeList;

auto filter_attribute_list_by(AttributeList const &attrs, ItemKind item_kind);

auto parse_variable_declaration(Parser &parser, AttributeList attrs)
    -> std::optional<std::unique_ptr<VariableDeclaration>>;
auto parse_compound_statement(Parser &parser)
	-> std::optional<std::unique_ptr<CompoundStatement>>;
