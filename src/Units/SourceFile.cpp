#include "Units/SourceFile.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <print>
#include <string>
#include <string_view>

SourceFile::SourceFile(std::string const &path)
    : m_path(std::move(path)), m_lineStarts() {
    std::fstream file{m_path, std::ios::binary | std::ios::in};
    assert(file.is_open());
    new (&m_content) std::string{(std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>()};
    usize line = 0;
    for (char const ch : m_content) {
        if (ch == '\n') {
            line += 1;
        }
        m_lineStarts.push_back(line);
    }
}

