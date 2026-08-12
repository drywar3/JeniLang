#include "entity/function.hpp"
#include "entity/symbol.hpp"
#include "lexer/source_location.hpp"
#include <string>

FunctionSymbol::FunctionSymbol(std::string &&name, SourceLocation definition)
    : Symbol(name, definition) {

}