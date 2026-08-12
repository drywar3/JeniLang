#include "parser/parser.hpp"
#include "compiler.hpp"
#include "diagnostic.hpp"
#include "lexer/lexer.hpp"
#include "lexer/source_location.hpp"
#include "lexer/token.hpp"
#include "parser/token_stream.hpp"
#include "common/common.hpp"
#include <optional>
#include <string_view>
#include <vector>

auto TokenStream::init(DiagnosticPool &diagnostics, std::string_view source,
                       SourceId source_id) -> TokenStream {
    Lexer lexer{source, source_id};
    std::vector<Token> tokens;
    while (!lexer.is_done()) {
        if (auto result = lexer.next_token(); result.has_value()) {
            tokens.push_back(result.value());
        } else {
            diagnostics.emit(result.error());
        }
    }
    return {.index = 0, .tokens = tokens};
}

Parser::Parser(PackageId package_id, DiagnosticPool &diagnostics, Compiler &compiler,
               std::string_view source, SourceId source_id)
    : m_package_id(package_id), m_diagnostics(diagnostics), m_compiler(compiler), m_source(source),
      m_source_id(source_id),
      m_token_stream(TokenStream::init(m_diagnostics, m_source, m_source_id)) {}

auto Parser::current(this Parser const &self) -> Token {
    if (self.m_token_stream.is_done())
        return self.m_token_stream.tokens.back();
    return self.m_token_stream.peek(0);
}

auto Parser::previous(this Parser const &self) -> Token {
    if (self.m_token_stream.is_done())
        return self.m_token_stream.tokens.back();
    return self.m_token_stream.peek(-1);
}

auto Parser::next(this Parser &self) -> Token {
    auto token = self.current();
    self.m_token_stream.index += 1;
    return token;
}

auto Parser::eat(this Parser &self, TokenKind kind) -> bool {
    if (!self.is(kind)) {
        TODO();
    }

    self.next();
    return true;
}

auto Parser::eat_one_of(this Parser &self,
                        std::initializer_list<TokenKind> kinds)
    -> std::optional<Token> {
    for (auto const &token : kinds) {
        if (self.is(token)) {
            self.next();
            return self.previous();
        }
    }
    printf("expected: [");
    for (auto const &token : kinds) {
        printf("`%s`, ", to_string(token).data());
    }
    printf("]\n");
    TODO();
    return std::nullopt;
}

auto Parser::eat_sequence(this Parser &self,
                          std::initializer_list<TokenKind> sequence) -> bool {
    TODO();
}

auto Parser::try_eat(this Parser &self, TokenKind kind) -> bool {
    if (!self.is(kind)) {
        return false;
    }

    self.next();
    return true;
}

auto Parser::try_eat_one_of(this Parser &self,
                            std::initializer_list<TokenKind> kinds)
    -> std::optional<Token> {
    for (auto const &token : kinds) {
        if (self.is(token)) {
            self.next();
            return self.previous();
        }
    }
    return std::nullopt;
}

auto Parser::try_eat_sequence(this Parser &self,
                              std::initializer_list<TokenKind> sequence)
    -> bool {
    TODO();
}

auto Parser::is(this Parser const &self, TokenKind kind) -> bool {
    return self.current().kind == kind;
}
auto Parser::is_one_of(this Parser const &self,
                       std::initializer_list<TokenKind> kinds) -> bool {
    for (auto const &token : kinds)
        if (self.is(token))
            return true;
    return false;
}

auto Parser::is_sequence(this Parser const &self,
                         std::initializer_list<TokenKind> kinds) -> bool {
    int offset = 0;
    for (auto const &token : kinds) {
        if (self.m_token_stream.peek(offset).kind != token) {
            return false;
        }
        offset += 1;
    }
    return true;
}

auto Parser::slice(this Parser const &self, SourceLocation location)
    -> std::string_view {
    return self.m_compiler.get_source_file(location.id)
        ->content()
        .subview(location.begin, location.length());
}

auto Parser::skip_to(this Parser &self, TokenKind kind, bool one_past) -> bool {
    self.skip_while(
        [self, kind](auto tok) { return !self.is_done() and !self.is(kind); },
        one_past);
    return !self.is_done();
}

auto Parser::skip_to_one_of(this Parser &self,
                            std::initializer_list<TokenKind> kinds,
                            bool one_past) -> bool {
    self.skip_while([self, kinds](auto tok) {
        return !self.is_done() and ({
            bool yes = false;
            for (auto const &kind : kinds) {
                if (tok.kind == kind)
                    yes = true;
            }
            yes;
        });
    });
    return !self.is_done();
}
