#include "Entity/Namespace.hpp"
#include "Lexer/SourceLocation.hpp"
#include <string>
#include <string_view>

Symbol::Symbol(std::string const &name, SourceLocation definition)
    : m_name(std::move(name)), m_definition(definition) {}

auto Symbol::GetName(this Symbol const &self) -> std::string_view {
    return self.m_name.subview();
}
