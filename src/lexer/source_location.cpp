#include "lexer/source_location.hpp"
#include <cassert>

auto SourceLocation::merge(this SourceLocation const &self,
                           SourceLocation const &other) -> SourceLocation {
    assert(self.id.id == other.id.id);
    return SourceLocation(self.id, self.first_line, other.last_line, self.begin,
                          other.end);
}
