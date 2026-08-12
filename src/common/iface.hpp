#pragma once

#include <sstream>

struct Stringable {
    public:
        virtual ~Stringable() = default;
        virtual auto to_string(std::stringstream &) const -> void = 0;
};
