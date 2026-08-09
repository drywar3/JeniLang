#pragma once

#include "diagnostic.hpp"
#include "lexer/source_location.hpp"
#include "lexer/token.hpp"
#include "units/source_file.hpp"
#include "common/common.hpp"
#include <expected>
#include <string_view>

struct Lexer {
  public:
    Lexer(std::string_view source, SourceId source_id);

    auto source(this Lexer const &self) { return self.m_source; }

    auto next_token(this Lexer &self) -> std::expected<Token, Diagnostic>;

    auto prefix(this Lexer const &self, std::string_view prefix) -> bool;
    auto eat_prefix(this Lexer &self, std::string_view prefix) -> bool;

    auto next(this Lexer &self) -> char;
    auto current(this Lexer const &self) -> char;

    auto is_done(this Lexer const &self) -> bool;

    auto get_location(this Lexer const &self) -> SourceLocation;
    auto get_token(this Lexer const &self, TokenKind kind) -> Token;

  private:
    std::string_view m_source;
    SourceId m_source_id;
    usize m_offset;
    usize m_line, m_prev_line, m_column, m_prev_column;
};
