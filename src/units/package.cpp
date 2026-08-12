#include "units/package.hpp"
#include "ast/ast.hpp"
#include "entity/namespace.hpp"
#include "lexer/source_location.hpp"
#include <memory>
#include <string>

Package::Package(std::string const &name, SourceId of_root_file)
    : m_name(name), m_root(0), m_root_source_file(of_root_file) {
    m_asts.push_back({});
    SymbolRef root_namespace = std::make_unique<Namespace>(Namespace::with_ast(
                "root", SourceLocation::beginning(of_root_file), AstId(0)));
    Scope root_scope{std::nullopt, Scope::Kind::Global};
    m_root = root_scope.put("root", std::move(root_namespace));
    m_scopes.push_back(std::move(root_scope));
}

auto Package::get_root_id(this Package const &self) -> SymbolId {
    return self.m_root;
}

auto Package::get_root(this Package &self) -> SymbolRef * {
    Scope &root_scope = self.m_scopes[GLOBAL_SCOPE.id];
    auto *symbol = root_scope.get_from_id(SymbolId(0));
    return symbol;
}

auto Package::get_root(this Package const &self) -> SymbolRef const * { TODO(); }

auto Package::get_root_source_file(this Package const &self) -> SourceId {
    return self.m_root_source_file;
}

auto Package::get_ast(this Package &self, AstId id) -> Ast & {
    return self.m_asts[id.id];
}

auto Package::get_scope(this Package const &self, ScopeId scope_id) -> Scope const & {
    return self.m_scopes[scope_id.id];
}

auto Package::get_scope(this Package &self, ScopeId scope_id) -> Scope & {
    return self.m_scopes[scope_id.id];
}
