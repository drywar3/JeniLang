#include "ast/item.hpp"
#include "defs.hpp"
#include <algorithm>
#include <utility>

FunctionDeclaration::FunctionDeclaration(
    SourceLocation source_location, AttributeList const &attrs, Name name,
    FunctionPrototype &prototype, Effects effects,
    std::optional<StatementRef> &body)
    : Item(source_location, ItemKind::FunctionDeclaration, attrs), m_name(name),
      m_prototype(std::move(prototype)), m_effects(std::move(effects)),
m_body(std::move(body)) { }
