#include "ast/defs.hpp"
#include "ast/misc.hpp"
#include <sstream>
#include <string_view>

auto FunctionDeclaration::to_string(std::stringstream &ss) const -> void {
    ss << "Function(" << m_name.name << ")\n";
    ss << "   params(" << m_prototype.parameters.size() << "):\n";
    for (auto const &param : m_prototype.parameters) {
        ss << "        " << param.name.name << "\n";
    }
}

auto FunctionDeclaration::get_name(this FunctionDeclaration const &self) -> Name const & {
    return self.m_name;
}

auto VariableDeclaration::to_string(std::stringstream &ss) const -> void {
    ss << "Variable(" << m_name.name << ")\n";
}
