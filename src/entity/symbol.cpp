#include "entity/namespace.hpp"
#include "lexer/source_location.hpp"
#include <string>
#include <string_view>

Symbol::Symbol(std::string const &name, SourceLocation definition)
    : m_name(std::move(name)), m_definition(definition) {}

auto Symbol::get_name(this Symbol const &self) -> std::string_view {
    return self.m_name.subview();
}
