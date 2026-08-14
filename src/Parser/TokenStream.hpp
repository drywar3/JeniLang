#pragma once

#include "Diagnostic.hpp"
#include "Lexer/Token.hpp"
#include "Common/this.hpp"
#include <string_view>
#include <vector>

struct TokenStream {
  public:
    static auto Init(DiagnosticPool &diagnostics, std::string_view source,
                     SourceId source_id) -> TokenStream;
    usize index;
    std::vector<Token> tokens;

    auto IsDone(this TokenStream const &self) -> bool {
        return self.index >= self.tokens.size();
    }

    auto Nth(this TokenStream const &self, usize n) {
        if (n >= self.tokens.size())
            return self.tokens.back();
        return self.tokens[n];
    }

    auto Peek(this TokenStream const &self, i32 n) {
        if (self.IsDone())
            return self.tokens.back();
        return self.Nth(self.index + n);
    }
};
