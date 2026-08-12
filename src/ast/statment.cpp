#include "ast/statement.hpp"
#include "defs.hpp"

CompoundStatement::CompoundStatement(SourceLocation begin, SourceLocation end, Body &body) 
 : Statement(begin.merge(end), StatementKind::CompoundStatement),
   m_body(std::move(body)), m_begin(begin), m_end(end) {

}