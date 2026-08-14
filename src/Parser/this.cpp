#include "Parser/this.hpp"
#include "Compiler.hpp"
#include "Diagnostic.hpp"
#include "Lexer/this.hpp"
#include "Lexer/SourceLocation.hpp"
#include "Lexer/Token.hpp"
#include "Parser/TokenStream.hpp"
#include "Common/this.hpp"
#include <optional>
#include <string_view>
#include <vector>

auto TokenStream::Init(DiagnosticPool &diagnostics, std::string_view source,
                       SourceId sourceId) -> TokenStream {
    Lexer lexer{source, sourceId};
    std::vector<Token> tokens;
    while (!lexer.IsDone()) {
        if (auto result = lexer.NextToken(); result.has_value()) {
            tokens.push_back(result.value());
        } else {
            diagnostics.Emit(result.error());
        }
    }
    return {.index = 0, .tokens = tokens};
}

Parser::Parser(PackageId packageId, DiagnosticPool &diagnostics, Compiler &compiler,
               std::string_view source, SourceId sourceId)
    : m_packageId(packageId), m_diagnostics(diagnostics), m_compiler(compiler), m_source(source),
      m_sourceId(sourceId),
      m_tokenStream(TokenStream::Init(m_diagnostics, m_source, m_sourceId)) {}

auto Parser::Current(this Parser const &self) -> Token {
    if (self.m_tokenStream.IsDone())
        return self.m_tokenStream.tokens.back();
    return self.m_tokenStream.Peek(0);
}

auto Parser::Previous(this Parser const &self) -> Token {
    if (self.m_tokenStream.IsDone())
        return self.m_tokenStream.tokens.back();
    return self.m_tokenStream.Peek(-1);
}

auto Parser::Next(this Parser &self) -> Token {
    auto token = self.Current();
    self.m_tokenStream.index += 1;
    return token;
}

auto Parser::Eat(this Parser &self, TokenKind kind) -> bool {
    if (!self.Is(kind)) {
        TODO();
    }

    self.Next();
    return true;
}

auto Parser::EatOneOf(this Parser &self,
                        std::initializer_list<TokenKind> kinds)
    -> std::optional<Token> {
    for (auto const &token : kinds) {
        if (self.Is(token)) {
            self.Next();
            return self.Previous();
        }
    }
    printf("expected: [");
    for (auto const &token : kinds) {
        printf("`%s`, ", ToString(token).data());
    }
    printf("]\n");
    TODO();
    return std::nullopt;
}

auto Parser::EatSequence(this Parser &self,
                          std::initializer_list<TokenKind> sequence) -> bool {
    TODO();
}

auto Parser::TryEat(this Parser &self, TokenKind kind) -> bool {
    if (!self.Is(kind)) {
        return false;
    }

    self.Next();
    return true;
}

auto Parser::TryEatOneOf(this Parser &self,
                            std::initializer_list<TokenKind> kinds)
    -> std::optional<Token> {
    for (auto const &token : kinds) {
        if (self.Is(token)) {
            self.Next();
            return self.Previous();
        }
    }
    return std::nullopt;
}

auto Parser::TryEatSequence(this Parser &self,
                              std::initializer_list<TokenKind> sequence)
    -> bool {
    TODO();
}

auto Parser::Is(this Parser const &self, TokenKind kind) -> bool {
    return self.Current().kind == kind;
}
auto Parser::IsOneOf(this Parser const &self,
                       std::initializer_list<TokenKind> kinds) -> bool {
    for (auto const &token : kinds)
        if (self.Is(token))
            return true;
    return false;
}

auto Parser::IsSequence(this Parser const &self,
                         std::initializer_list<TokenKind> kinds) -> bool {
    int offset = 0;
    for (auto const &token : kinds) {
        if (self.m_tokenStream.Peek(offset).kind != token) {
            return false;
        }
        offset += 1;
    }
    return true;
}

auto Parser::Slice(this Parser const &self, SourceLocation location)
    -> std::string_view {
    return self.m_compiler.GetSourceFile(location.id)
        ->Content()
        .subview(location.begin, location.Length());
}

auto Parser::SkipTo(this Parser &self, TokenKind kind, bool one_past) -> bool {
    self.SkipWhile(
        [self, kind](auto tok) { return !self.IsDone() and !self.Is(kind); },
        one_past);
    return !self.IsDone();
}

auto Parser::SkipToOneOf(this Parser &self,
                            std::initializer_list<TokenKind> kinds,
                            bool one_past) -> bool {
    self.SkipWhile([self, kinds](auto tok) {
        return !self.IsDone() and ({
            bool yes = false;
            for (auto const &kind : kinds) {
                if (tok.kind == kind)
                    yes = true;
            }
            yes;
        });
    });
    return !self.IsDone();
}
