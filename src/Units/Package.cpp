#include "Units/Package.hpp"
#include "Ast/this.hpp"
#include "Entity/Namespace.hpp"
#include "Lexer/SourceLocation.hpp"
#include <memory>
#include <string>


Package::Package(std::string const &name, SourceId ofRootFile)
    : m_name(name), m_root(0), m_rootSourceFile(ofRootFile) {
    m_asts.push_back({});
    SymbolRef rootNamespace = std::make_unique<Namespace>(Namespace::withAst(
                "root", SourceLocation::beginning(ofRootFile), AstId(0)));
    Scope rootScope{std::nullopt, Scope::Kind::Global};
    m_root = rootScope.Put("root", std::move(rootNamespace));
    m_scopes.push_back(std::move(rootScope));
}

auto Package::GetRootId(this Package const &self) -> SymbolId {
    return self.m_root;
}

auto Package::GetRoot(this Package &self) -> Symbol * {
    Scope &rootScope = self.m_scopes[GLOBAL_SCOPE.id];
    auto *symbol = rootScope.GetFromId(SymbolId(0));
    return symbol;
}

auto Package::GetRoot(this Package const &self) -> Symbol const * { TODO(); }

auto Package::GetRootSourceFile(this Package const &self) -> SourceId {
    return self.m_rootSourceFile;
}

auto Package::GetAst(this Package &self, AstId id) -> Ast & {
    return self.m_asts[id.id];
}

auto Package::GetScope(this Package const &self, ScopeId scopeId) -> Scope const & {
    return self.m_scopes[scopeId.id];
}

auto Package::GetScope(this Package &self, ScopeId scopeId) -> Scope & {
    return self.m_scopes[scopeId.id];
}
