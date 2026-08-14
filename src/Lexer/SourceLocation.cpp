#include "Lexer/SourceLocation.hpp"
#include <cassert>

auto SourceLocation::Merge(this SourceLocation const &self,
                           SourceLocation const &other) -> SourceLocation {
    assert(self.id.id == other.id.id);
    return SourceLocation(self.id, self.begin, other.end);
}
