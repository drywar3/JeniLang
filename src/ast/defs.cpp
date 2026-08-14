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

auto FunctionDeclaration::get_prototype(this FunctionDeclaration const &self) -> FunctionPrototype const& {
    return self.m_prototype;
}

auto VariableDeclaration::to_string(std::stringstream &ss) const -> void {
    ss << "Variable(" << m_name.name << ")\n";
}

auto FunctionDeclaration::get_body(this FunctionDeclaration &self) -> Statement * {
    if (self.m_body.has_value()) return self.m_body->get();
    return nullptr;
}

auto FunctionDeclaration::get_body(this FunctionDeclaration const &self) -> Statement const* {
    if (self.m_body) return self.m_body->get();
    return nullptr;
}

auto CompoundStatement::get_body(this CompoundStatement const &self) -> std::vector<StatementRef> const & {
    return self.m_body;
}