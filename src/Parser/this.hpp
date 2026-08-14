#pragma once

#include "Compiler.hpp"
#include "Diagnostic.hpp"
#include "Lexer/SourceLocation.hpp"
#include "Lexer/Token.hpp"
#include "Entity/Namespace.hpp"
#include "Parser/TokenStream.hpp"
#include "Units/Package.hpp"
#include <initializer_list>
#include <optional>
#include <string_view>

struct Parser {
  public:
    Parser(PackageId, DiagnosticPool &, Compiler &, std::string_view,
           SourceId);

    auto Current(this Parser const &self) -> Token;
    auto Previous(this Parser const &self) -> Token;

    auto Next(this Parser &self) -> Token;

    auto Eat(this Parser &self, TokenKind kind) -> bool;
    auto EatOneOf(this Parser &self, std::initializer_list<TokenKind> kinds)
        -> std::optional<Token>;
    auto EatSequence(this Parser &self,
                      std::initializer_list<TokenKind> sequence) -> bool;

    auto TryEat(this Parser &self, TokenKind kind) -> bool;
    auto TryEatOneOf(this Parser &self,
                        std::initializer_list<TokenKind> kinds)
        -> std::optional<Token>;
    auto TryEatSequence(this Parser &self,
                          std::initializer_list<TokenKind> sequence) -> bool;

    auto Is(this Parser const &self, TokenKind kind) -> bool;
    auto IsOneOf(this Parser const &self,
                   std::initializer_list<TokenKind> kinds) -> bool;
    auto IsSequence(this Parser const &self,
                     std::initializer_list<TokenKind> kinds) -> bool;

    auto SkipTo(this Parser &self, TokenKind kind, bool onePast = false)
        -> bool;
    auto SkipToOneOf(this Parser &self,
                        std::initializer_list<TokenKind> kinds,
                        bool onePast = false) -> bool;

    template <typename Fn>
    auto SkipWhile(this Parser &self, Fn &&fn, bool onePast = false) -> bool {
        while (fn(self.Current())) {
            self.Next();
        }

        if (onePast and !self.IsDone())
            self.Next();
        return not self.IsDone();
    }

    auto IsDone(this Parser const &self) -> bool {
        return self.Current().kind == TokenKind::EndOfFile;
    }

    auto Slice(this Parser const &self, SourceLocation location)
        -> std::string_view;

    auto m_GetCC() { return &m_compiler; }
    auto GetPackage(this Parser const &self) -> PackageId { return self.m_packageId; }
  private:
    PackageId m_packageId;
    DiagnosticPool &m_diagnostics;
    Compiler &m_compiler;
    std::string_view m_source;
    SourceId m_sourceId;
    TokenStream m_tokenStream;
};

auto ParseIntoNamespace(Parser &, Namespace &out) -> void;
