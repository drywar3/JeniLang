#include "units/source_file.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <print>
#include <string>
#include <string_view>

SourceFile::SourceFile(std::string const &path)
    : m_path(std::move(path)), m_lines() {
    std::fstream file{m_path, std::ios::binary | std::ios::in};
    assert(file.is_open());

    new (&m_content) std::string{(std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>()};

    usize first   = 0;
    usize current = 0;
    while (current < m_content.size()) {
        if (m_content[current] == '\n') {
            usize len             = current - first;
            std::string_view line = m_content.subview(first, len);
            m_lines.push_back(line);
            first = current + 1;
        }
        current += 1;
    }

    if (current > first) {
        usize len             = current - first;
        std::string_view line = m_content.subview(first, len);
        m_lines.push_back(line);
    }
}

auto SourceFile::get_line(this SourceFile const &self, usize line)
    -> std::string_view const & {
    return self.m_lines.at(line);
}

auto SourceFile::lines(this SourceFile const &self)
    -> std::vector<std::string_view> const & {
    return self.m_lines;
}
