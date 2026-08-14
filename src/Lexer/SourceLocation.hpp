#pragma once

#include "Common/this.hpp"

struct SourceLocation {
  private:
    bool is_null;

  public:
    SourceId id;
    usize begin, end;

    SourceLocation() : is_null(true), id(0) {}

    explicit SourceLocation(SourceId id) : is_null(false), id(id) {}

    SourceLocation(SourceId id, usize begin, usize end)
        : is_null(false), id(id), begin(begin), end(end) {}

    static auto beginning(SourceId of_file) -> SourceLocation {
        return SourceLocation(of_file, 1, 1);
    }

    auto WithBegin(this SourceLocation self, usize col) {
        self.begin = col;
        return self;
    }

    auto WithEnd(this SourceLocation self, usize col) {
        self.end = col;
        return self;
    }

    auto WithId(this SourceLocation self, SourceId id) {
        self.id      = id;
        self.is_null = false;
        return self;
    }

    auto Merge(this SourceLocation const &self, SourceLocation const &other)
        -> SourceLocation;

    auto Length(this SourceLocation const &self) -> usize {
        return self.end - self.begin;
    }

    static auto NULLLOC() -> SourceLocation { return SourceLocation(); }
};
