#include "Entity/Function.hpp"
#include "Entity/Symbol.hpp"
#include "Lexer/SourceLocation.hpp"
#include <string>

FunctionSymbol::FunctionSymbol(std::string &&name, SourceLocation definition)
    : Symbol(name, definition) {

}

auto FunctionSymbol::PrototypeIsDefined(this FunctionSymbol const &self) -> bool {
    return self.m_prototype.has_value();
}
    
auto FunctionSymbol::GetPrototype(this FunctionSymbol const &self) -> concrete::FunctionPrototype const* {
    if (!self.PrototypeIsDefined()) return nullptr;
    return &self.m_prototype.value();
}

auto FunctionSymbol::GetPrototype(this FunctionSymbol &self) -> concrete::FunctionPrototype* {
    if (!self.PrototypeIsDefined()) return nullptr;
    return &self.m_prototype.value();
}

auto FunctionSymbol::SetPrototype(this FunctionSymbol &self, concrete::FunctionPrototype &prototype) -> void {
    self.m_prototype = std::move(prototype);
}