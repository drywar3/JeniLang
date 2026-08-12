#pragma once

#include "compiler.hpp"
#include "diagnostic.hpp"
#include "lexer/source_location.hpp"
#include "lexer/token.hpp"
#include "entity/namespace.hpp"
#include "parser/token_stream.hpp"
#include "units/package.hpp"
#include <initializer_list>
#include <optional>
#include <string_view>

struct Parser {
  public:
    Parser(PackageId, DiagnosticPool &, Compiler &, std::string_view,
           SourceId);

    auto current(this Parser const &self) -> Token;
    auto previous(this Parser const &self) -> Token;

    auto next(this Parser &self) -> Token;

    auto eat(this Parser &self, TokenKind kind) -> bool;
    auto eat_one_of(this Parser &self, std::initializer_list<TokenKind> kinds)
        -> std::optional<Token>;
    auto eat_sequence(this Parser &self,
                      std::initializer_list<TokenKind> sequence) -> bool;

    auto try_eat(this Parser &self, TokenKind kind) -> bool;
    auto try_eat_one_of(this Parser &self,
                        std::initializer_list<TokenKind> kinds)
        -> std::optional<Token>;
    auto try_eat_sequence(this Parser &self,
                          std::initializer_list<TokenKind> sequence) -> bool;

    auto is(this Parser const &self, TokenKind kind) -> bool;
    auto is_one_of(this Parser const &self,
                   std::initializer_list<TokenKind> kinds) -> bool;
    auto is_sequence(this Parser const &self,
                     std::initializer_list<TokenKind> kinds) -> bool;

    auto skip_to(this Parser &self, TokenKind kind, bool one_past = false)
        -> bool;
    auto skip_to_one_of(this Parser &self,
                        std::initializer_list<TokenKind> kinds,
                        bool one_past = false) -> bool;

    template <typename Fn>
    auto skip_while(this Parser &self, Fn &&fn, bool one_past = false) -> bool {
        while (fn(self.current())) {
            self.next();
        }

        if (one_past and !self.is_done())
            self.next();
        return not self.is_done();
    }

    auto is_done(this Parser const &self) -> bool {
        return self.current().kind == TokenKind::EndOfFile;
    }

    auto slice(this Parser const &self, SourceLocation location)
        -> std::string_view;

    auto m_get_cc() { return &m_compiler; }
    auto get_package(this Parser const &self) -> PackageId { return self.m_package_id; }
  private:
    PackageId m_package_id;
    DiagnosticPool &m_diagnostics;
    Compiler &m_compiler;
    std::string_view m_source;
    SourceId m_source_id;
    TokenStream m_token_stream;
};

auto parse_into_namespace(Parser &, Namespace &out) -> void;
