#pragma once

#include "ast/expression.hpp"
#include "ast/item.hpp"
#include "ast/misc.hpp"
#include "ast/statement.hpp"
#include "ast/type_hint.hpp"
#include "lexer/source_location.hpp"
#include <optional>
#include <sstream>
#include <vector>

struct VariableDeclaration : public Item, public Statement {
  public:
    VariableDeclaration(SourceLocation location, Mutability mutability,
                        AttributeList const &attrs)
        : Item(location, ItemKind::VariableDeclaration, attrs),
          Statement(location, StatementKind::VariableDeclaration),
          m_mutability(mutability) {}

    auto get_name(this VariableDeclaration &self) -> Name & {
        return self.m_name;
    }

    auto get_type_hint(this VariableDeclaration &self)
        -> std::optional<TypeHintRef> & {
        return self.m_type_hint;
    }

    auto get_initializer(this VariableDeclaration &self)
        -> std::optional<ExpressionRef> & {
        return self.m_initializer;
    }

    auto get_mutability(this VariableDeclaration &self) {
        return self.m_mutability;
    }

    auto to_string(std::stringstream &) const -> void override;

  private:
    Mutability m_mutability;
    Name m_name;
    std::optional<TypeHintRef> m_type_hint;
    std::optional<ExpressionRef> m_initializer;
};

enum FunctionParameterPolicy {
    Move,
    Copy,
};

struct FunctionParameter {
    FunctionParameterPolicy policy;
    Name name;
    std::optional<TypeHintRef> type_hint;
    std::optional<ExpressionRef> default_value;
};

using FunctionParameterList = std::vector<FunctionParameter>;

struct FunctionPrototype {
    TypeHintRef return_type;
    FunctionParameterList parameters;
};

struct FunctionEffect {
    enum struct Kind {
        Throws,
        Panics,
        Nullable,
    } kind;

    struct Throws {
    } throws;
};

struct FunctionDeclaration : public Item {
  public:
    using Effects = std::vector<FunctionEffect>;

    FunctionDeclaration(SourceLocation source_location,
                        AttributeList const &attrs, Name name,
                        FunctionPrototype &prototype, Effects effects,
                        std::optional<StatementRef> &body);

    auto get_name(this FunctionDeclaration &) -> Name &;
    auto get_name(this FunctionDeclaration const &) -> Name const &;

    auto get_prototype(this FunctionDeclaration &) -> FunctionPrototype &;
    auto get_prototype(this FunctionDeclaration const &)
        -> FunctionPrototype const &;

    auto to_string(std::stringstream &) const -> void override;
  private:
    Name m_name;
    FunctionPrototype m_prototype;
    FunctionDeclaration::Effects m_effects;
    std::optional<StatementRef> m_body;
};

struct CompoundStatement: public Statement {
public:
    using Body = std::vector<StatementRef>;

    CompoundStatement(SourceLocation begin, SourceLocation end, Body &body);
private:
    Body m_body;
    SourceLocation m_begin, m_end;
};
