#include "entity/function.hpp"
#include "entity/symbol.hpp"
#include "lexer/source_location.hpp"
#include <string>

FunctionSymbol::FunctionSymbol(std::string &&name, SourceLocation definition)
    : Symbol(name, definition) {

}

auto FunctionSymbol::prototype_is_defined(this FunctionSymbol const &self) -> bool {
    return self.m_prototype.has_value();
}
    
auto FunctionSymbol::get_prototype(this FunctionSymbol const &self) -> concrete::FunctionPrototype const* {
    if (!self.prototype_is_defined()) return nullptr;
    return &self.m_prototype.value();
}

auto FunctionSymbol::get_prototype(this FunctionSymbol &self) -> concrete::FunctionPrototype* {
    if (!self.prototype_is_defined()) return nullptr;
    return &self.m_prototype.value();
}

auto FunctionSymbol::set_prototype(this FunctionSymbol &self, concrete::FunctionPrototype &prototype) -> void {
    self.m_prototype = std::move(prototype);
}