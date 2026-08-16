#include "Ast/Statement.hpp"
#include "Ast/Defs.hpp"
#include "Lexer/SourceLocation.hpp"

CompoundStatement::CompoundStatement(SourceLocation begin, SourceLocation end,
                                     Body &body)
    : Statement(begin.Merge(end), StatementKind::CompoundStatement),
      m_body(std::move(body)), m_begin(begin), m_end(end)
{
}

auto Statement::GetLocation(this Statement const &self) -> SourceLocation
{
    return self.m_location;
}