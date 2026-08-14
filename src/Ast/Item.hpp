#pragma once

#include "Ast/Misc.hpp"
#include "Common/Iface.hpp"
#include "Lexer/SourceLocation.hpp"
#include <memory>

enum struct ItemKind {
    VariableDeclaration,
    FunctionDeclaration,
    PackageImport,
    UsingItem,
    NamespaceDeclaration,
    NamespaceDefinition,
    TypeAlias,
    InterfaceDefinition,
    StructDefinition,
    EnumDefinition,
    TypeExtension,
};

class Item : public Stringable {
  public:
    Item(SourceLocation location, ItemKind kind, AttributeList const& attrs)
        : m_location(location), m_kind(kind), m_attrs(std::move(attrs)) {}
    virtual ~Item() = default;

    auto GetKind() const -> ItemKind { return m_kind; }
    auto GetAttributes(this Item const& self) -> AttributeList const& { return self.m_attrs; }

  private:
    SourceLocation m_location;
    ItemKind m_kind;
    AttributeList m_attrs;
};

using ItemRef = std::unique_ptr<Item>;
