#pragma once

#include "lexer/source_location.hpp"
#include <string_view>
#include <vector>

struct Name {
    std::string_view name;
    SourceLocation location;
};

struct Attribute {
    enum struct Kind {
        Extern,
        Inline,
        NoInline,
        Constexpr,
        NoConstexpr,
        Packed,
        Link,
    };
};

using AttributeList = std::vector<Attribute>;
