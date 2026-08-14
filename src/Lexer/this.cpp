#include "Lexer/this.hpp"
#include "Lexer/SourceLocation.hpp"
#include "Lexer/Token.hpp"
#include <cctype>
#include <initializer_list>

Lexer::Lexer(std::string_view Source, SourceId source_id)
    : m_source(Source), m_sourceId(source_id), m_offset(0), m_prevOffset(0) {}

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

auto Lexer::GetLocation(this Lexer const &self) -> SourceLocation {
    return SourceLocation(self.m_sourceId)
        .WithBegin(self.m_prevOffset)
        .WithEnd(self.m_offset);
}

auto Lexer::GetToken(this Lexer const &self, TokenKind kind) -> Token {
    return Token(kind, self.GetLocation());
}

auto Lexer::NextToken(this Lexer &self) -> std::expected<Token, Diagnostic> {
    static constexpr std::string_view WHITESPACE = "\n\t\r ";

    if (self.IsDone()) {
        return self.GetToken(TokenKind::EndOfFile);
    }

    while (true) {
        if (WHITESPACE.contains(self.Current())) {
            self.Next();
            continue;
        }

        if (self.Prefix("//")) {
            while (!self.IsDone() and self.Current() != '\n') {
                self.Next();
            }
            continue;
        }

        if (self.Prefix("[*")) {
            TODO("DOC COMMENT");
        }

        break;
    }

    if (self.IsDone()) {
        return self.GetToken(TokenKind::EndOfFile);
    }

    self.m_prevOffset = self.m_offset;

    if (std::isalpha(self.Current()) or self.Current() == '_') {
        usize begin = self.m_offset;
        while (std::isalnum(self.Current()) or self.Current() == '_') {
            self.Next();
        }
        usize len             = self.m_offset - begin;
        std::string_view word = self.Source().subview(begin, len);
        for (auto const &[lexeme, token] : KEYWORDS) {
            if (word == lexeme)
                return self.GetToken(token);
        }

        return self.GetToken(TokenKind::Identifier);
    }

    if (std::isdigit(self.Current())) {
        while (std::isdigit(self.Current())) {
            self.Next();
        }
        return self.GetToken(TokenKind::IntegerLiteral);
    }

    for (auto const &[lexeme, token] : PUNCTS) {
        if (self.EatPrefix(lexeme)) {
            return self.GetToken(token);
        }
    }

    TODO("%s", self.Source().subview(self.m_offset).data());
}

auto Lexer::Prefix(this Lexer const &self, std::string_view Prefix) -> bool {
    return self.Source().subview(self.m_offset).starts_with(Prefix);
}

auto Lexer::EatPrefix(this Lexer &self, std::string_view Prefix) -> bool {
    if (self.Prefix(Prefix)) {
        for (usize n = 0; n < Prefix.size(); ++n)
            self.Next();
        return true;
    }

    return false;
}

auto Lexer::Next(this Lexer &self) -> char {
    char ch = self.Current();
    self.m_offset += 1;
    return ch;
}

auto Lexer::Current(this Lexer const &self) -> char {
    if (self.IsDone())
        return '\0';
    return self.Source().at(self.m_offset);
}

auto Lexer::IsDone(this Lexer const &self) -> bool {
    return self.m_offset >= self.Source().size();
}
