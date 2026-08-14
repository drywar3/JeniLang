#pragma once

#include "common/common.hpp"
#include "lexer/source_location.hpp"
#include <expected>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

template<typename T>
using MaybeUndefined = std::optional<T>;

template<typename T, typename With>
using MayError = std::expected<T, With>;

class Symbol {
  public:
    Symbol(std::string const &name, SourceLocation definition);
    virtual ~Symbol() = default;

    auto get_name(this Symbol const &self) -> std::string_view;
    auto get_definition(this Symbol const &self) -> SourceLocation;

  private:
    std::string m_name;
    SourceLocation m_definition;
};

using SymbolRef = std::unique_ptr<Symbol>;

struct SymbolId {
    usize id;
    explicit SymbolId(usize id) : id(id) {}
};
