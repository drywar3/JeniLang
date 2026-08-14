#pragma once

#include <sstream>

struct Stringable {
    public:
        virtual ~Stringable() = default;
        virtual auto ToString(std::stringstream &) const -> void = 0;
};
