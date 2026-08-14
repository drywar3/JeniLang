#pragma once

#include "Common/this.hpp"
#include <string>
#include <string_view>
#include <vector>

struct SourceFile {
  public:
    SourceFile(std::string const &path);

    auto Path(this SourceFile const &self) { return self.m_path.subview(); }
    auto Content(this SourceFile const &self) {
        return self.m_content.subview();
    }

  private:
    std::string m_content;
    std::string m_path;
    std::vector<usize> m_lineStarts;
};

bool OpenSourceFile(SourceFile &out, std::string path);
