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

  private:
    std::string m_content;
    std::string m_path;
    std::vector<usize> m_line_starts;
};

bool open_source_file(SourceFile &out, std::string path);
