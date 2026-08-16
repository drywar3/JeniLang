#pragma once

#include "Ast/Expression.hpp"
#include "Ast/Item.hpp"
#include "Ast/Misc.hpp"
#include "Ast/Statement.hpp"
#include "Ast/TypeHint.hpp"
#include "Lexer/SourceLocation.hpp"
#include <optional>
#include <sstream>

struct VariableDeclaration : public Item, public Statement {
  public:
    VariableDeclaration(SourceLocation location, Mutability mutability,
                        AttributeList const &attrs)
        : Item(location, ItemKind::VariableDeclaration, attrs),
          Statement(location, StatementKind::VariableDeclaration),
          m_mutability(mutability) {}

    auto GetName(this VariableDeclaration &self) -> Name & {
        return self.m_name;
    }

    auto GetName(this VariableDeclaration const &self) -> Name const & {
        return self.m_name;
    }

    auto GetTypeHint(this VariableDeclaration &self)
        -> std::optional<TypeHintRef> & {
        return self.m_type_hint;
    }

    auto GetInitializer(this VariableDeclaration &self)
        -> std::optional<ExpressionRef> & {
        return self.m_initializer;
    }

    auto get_mutability(this VariableDeclaration &self) {
        return self.m_mutability;
    }

    auto ToString(std::stringstream &) const -> void override;

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
    std::optional<TypeHintRef> typeHint;
    std::optional<ExpressionRef> defaultValue;
};

using FunctionParameterList = std::vector<FunctionParameter>;

struct FunctionPrototype {
    TypeHintRef returnType;
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

    auto GetName(this FunctionDeclaration &) -> Name &;
    auto GetName(this FunctionDeclaration const &) -> Name const &;

    auto GetPrototype(this FunctionDeclaration &) -> FunctionPrototype &;
    auto GetPrototype(this FunctionDeclaration const &)
        -> FunctionPrototype const &;

    auto GetBody(this FunctionDeclaration &) -> Statement *;
    auto GetBody(this FunctionDeclaration const &) -> Statement const*;

    auto ToString(std::stringstream &) const -> void override;
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

    auto GetBody(this CompoundStatement const &) -> std::vector<StatementRef> const &;
private:
    Body m_body;
    SourceLocation m_begin, m_end;
};
