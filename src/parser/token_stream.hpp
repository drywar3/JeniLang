#pragma once

#include "diagnostic.hpp"
#include "lexer/token.hpp"
#include "units/source_file.hpp"
#include "common/common.hpp"
#include <string_view>
#include <vector>

struct TokenStream {
  public:
    static auto init(DiagnosticPool &diagnostics, std::string_view source,
                     SourceId source_id) -> TokenStream;
    usize index;
    std::vector<Token> tokens;

    auto is_done(this TokenStream const &self) -> bool {
        return self.index >= self.tokens.size();
    }

    auto nth(this TokenStream const &self, usize n) {
        if (n >= self.tokens.size())
            return self.tokens.back();
        return self.tokens[n];
    }

    auto peek(this TokenStream const &self, i32 n) {
        if (self.is_done())
            return self.tokens.back();
        return self.nth(self.index + n);
    }
};
