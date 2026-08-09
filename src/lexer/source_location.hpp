#pragma once

#include "common/common.hpp"

struct SourceLocation {
  private:
    bool is_null;

  public:
    SourceId id;
    usize first_line, last_line;
    usize begin, end;

    SourceLocation() : is_null(true), id(0) {}

    explicit SourceLocation(SourceId id) : is_null(false), id(id) {}

    SourceLocation(SourceId id, usize first_line, usize last_line, usize begin,
                   usize end)
        : is_null(false), id(id), first_line(first_line), last_line(last_line),
          begin(begin), end(end) {}

    static auto beginning(SourceId of_file) -> SourceLocation {
        return SourceLocation(of_file, 1, 1, 1, 1);
    }

    auto fline(this SourceLocation self, usize line) {
        self.first_line = line;
        return self;
    }
    auto lline(this SourceLocation self, usize line) {
        self.last_line = line;
        return self;
    }
    auto fcol(this SourceLocation self, usize col) {
        self.begin = col;
        return self;
    }
    auto lcol(this SourceLocation self, usize col) {
        self.end = col;
        return self;
    }
    auto with_id(this SourceLocation self, SourceId id) {
        self.id      = id;
        self.is_null = false;
        return self;
    }

    auto merge(this SourceLocation const &self, SourceLocation const &other)
        -> SourceLocation;
    auto length(this SourceLocation const &self) -> usize {
        return self.end - self.begin;
    }

    static auto NULLLOC() -> SourceLocation { return SourceLocation(); }
};
