#pragma once

#include "common/common.hpp"
#include <string>
#include <string_view>
#include <vector>

struct SourceFile {
  public:
    SourceFile(std::string const &path);

    auto path(this SourceFile const &self) { return self.m_path.subview(); }
    auto content(this SourceFile const &self) {
        return self.m_content.subview();
    }
    auto lines(this SourceFile const &self)
        -> std::vector<std::string_view> const &;

    auto get_line(this SourceFile const &self, usize line)
        -> std::string_view const &;

  private:
    std::string m_content;
    std::string m_path;
    std::vector<std::string_view> m_lines;
};

bool open_source_file(SourceFile &out, std::string path);
