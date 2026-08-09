#include "entity/namespace.hpp"

Namespace::Namespace(std::string const &name, SourceLocation definition)
    : Symbol(std::move(name), definition), m_ast() {}
