#include "lexer/lexer.hpp"
#include "lexer/source_location.hpp"
#include "lexer/token.hpp"
#include <cctype>
#include <cstdio>
#include <initializer_list>
#include <string_view>

Lexer::Lexer(std::string_view source, SourceId source_id)
    : m_source(source), m_source_id(source_id), m_offset(0), m_prev_offset(0) {}

struct TokenSpec {
    std::string_view text;
    TokenKind token;
};

static constexpr std::initializer_list<TokenSpec> KEYWORDS = {
    {"func", TokenKind::KwFunc},
    {"var", TokenKind::KwVar},
    {"const", TokenKind::KwConst},
    {"comptime", TokenKind::KwComptime},
    {"return", TokenKind::KwReturn},
    {"requires", TokenKind::KwRequires},
    {"import", TokenKind::KwImport},
    {"namespace", TokenKind::KwNamespace},
    {"using", TokenKind::KwUsing},
    {"type", TokenKind::KwType},
    {"struct", TokenKind::KwStruct},
    {"enum", TokenKind::KwEnum},
    {"interface", TokenKind::KwInterface},
    {"extend", TokenKind::KwExtend},
    {"throws", TokenKind::KwThrows},
    {"throw", TokenKind::KwThrow},
    {"panics", TokenKind::KwPanics},
    {"panic", TokenKind::KwPanic},
    {"orelse", TokenKind::KwOrelse},
    {"operator", TokenKind::KwOperator},
    {"nullable", TokenKind::KwNullable},
    {"catch", TokenKind::KwCatch},
    {"todo", TokenKind::KwTodo},
    {"keep", TokenKind::KwKeep},
    {"label", TokenKind::KwLabel},
    {"if", TokenKind::KwIf},
    {"for", TokenKind::KwFor},
    {"callable", TokenKind::TkCallable},
    {"while", TokenKind::KwWhile},
    {"break", TokenKind::KwBreak},
    {"continue", TokenKind::KwContinue},

    {"int", TokenKind::TkInt},
    {"uint", TokenKind::TkUint},
    {"int8", TokenKind::TkInt8},
    {"uint8", TokenKind::TkUint8},
    {"int16", TokenKind::TkInt16},
    {"uint16", TokenKind::TkUint16},
    {"int32", TokenKind::TkInt32},
    {"uint32", TokenKind::TkUint32},
    {"int64", TokenKind::TkInt64},
    {"uint64", TokenKind::TkUint64},
    {"usize", TokenKind::TkUsize},
    {"isize", TokenKind::TkIsize},
    {"char", TokenKind::TkChar},
    {"string", TokenKind::TkString},
    {"void", TokenKind::TkVoid},
    {"bool", TokenKind::TkBool}};

static constexpr std::initializer_list<TokenSpec> PUNCTS = {
    {"->", TokenKind::OpArrow},       {"==", TokenKind::OpEquals},
    {"!=", TokenKind::OpNotEquals},   {"::", TokenKind::SpNamespace},

    {"=", TokenKind::OpAssign},       {"+", TokenKind::OpAdd},
    {"-", TokenKind::OpSub},          {"*", TokenKind::OpStar},
    {"/", TokenKind::OpSlash},        {";", TokenKind::SpSemicolon},
    {",", TokenKind::SpComma},        {":", TokenKind::SpColon},
    {".", TokenKind::SpDot},          {"(", TokenKind::SpOpenParen},
    {"[", TokenKind::SpOpenBracket},  {"{", TokenKind::SpOpenBrace},
    {")", TokenKind::SpCloseParen},   {"]", TokenKind::SpCloseBracket},
    {"}", TokenKind::SpCloseBrace},

    {"&", TokenKind::OpAmp},
};

auto Lexer::get_location(this Lexer const &self) -> SourceLocation {
    return SourceLocation(self.m_source_id)
        .with_begin(self.m_prev_offset)
        .with_end(self.m_offset);
}

auto Lexer::get_token(this Lexer const &self, TokenKind kind) -> Token {
    return Token(kind, self.get_location());
}

auto Lexer::next_token(this Lexer &self) -> std::expected<Token, Diagnostic> {
    static constexpr std::string_view WHITESPACE = "\n\t\r ";

    if (self.is_done()) {
        return self.get_token(TokenKind::EndOfFile);
    }

    while (true) {
        if (WHITESPACE.contains(self.current())) {
            self.next();
            continue;
        }

        if (self.prefix("//")) {
            while (!self.is_done() and self.current() != '\n') {
                self.next();
            }
            continue;
        }

        if (self.prefix("[*")) {
            TODO("DOC COMMENT");
        }

        break;
    }

    if (self.is_done()) {
        return self.get_token(TokenKind::EndOfFile);
    }

    self.m_prev_offset = self.m_offset;

    if (std::isalpha(self.current()) or self.current() == '_') {
        usize begin = self.m_offset;
        while (std::isalnum(self.current()) or self.current() == '_') {
            self.next();
        }
        usize len             = self.m_offset - begin;
        std::string_view word = self.source().subview(begin, len);
        for (auto const &[lexeme, token] : KEYWORDS) {
            if (word == lexeme)
                return self.get_token(token);
        }

        return self.get_token(TokenKind::Identifier);
    }

    if (std::isdigit(self.current())) {
        while (std::isdigit(self.current())) {
            self.next();
        }
        return self.get_token(TokenKind::IntegerLiteral);
    }

    for (auto const &[lexeme, token] : PUNCTS) {
        if (self.eat_prefix(lexeme)) {
            return self.get_token(token);
        }
    }

    TODO("%s", self.source().subview(self.m_offset).data());
}

auto Lexer::prefix(this Lexer const &self, std::string_view prefix) -> bool {
    return self.source().subview(self.m_offset).starts_with(prefix);
}

auto Lexer::eat_prefix(this Lexer &self, std::string_view prefix) -> bool {
    if (self.prefix(prefix)) {
        for (usize n = 0; n < prefix.size(); ++n)
            self.next();
        return true;
    }

    return false;
}

auto Lexer::next(this Lexer &self) -> char {
    char ch = self.current();
    self.m_offset += 1;
    return ch;
}

auto Lexer::current(this Lexer const &self) -> char {
    if (self.is_done())
        return '\0';
    return self.source().at(self.m_offset);
}

auto Lexer::is_done(this Lexer const &self) -> bool {
    return self.m_offset >= self.source().size();
}
