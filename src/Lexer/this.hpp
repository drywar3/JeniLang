#pragma once

#include "Diagnostic.hpp"
#include "Lexer/SourceLocation.hpp"
#include "Lexer/Token.hpp"
#include "Units/SourceFile.hpp"
#include "Common/this.hpp"
#include <expected>
#include <string_view>

struct Lexer {
  public:
    Lexer(std::string_view source, SourceId source_id);

    auto Source(this Lexer const &self) { return self.m_source; }

    auto NextToken(this Lexer &self) -> std::expected<Token, Diagnostic>;

    auto Prefix(this Lexer const &self, std::string_view prefix) -> bool;
    auto EatPrefix(this Lexer &self, std::string_view prefix) -> bool;

    auto Next(this Lexer &self) -> char;
    auto Current(this Lexer const &self) -> char;

    auto IsDone(this Lexer const &self) -> bool;

    auto GetLocation(this Lexer const &self) -> SourceLocation;
    auto GetToken(this Lexer const &self, TokenKind kind) -> Token;

  private:
    std::string_view m_source;
    SourceId m_sourceId;
    usize m_offset, m_prevOffset;
};
