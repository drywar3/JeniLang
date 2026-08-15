#include "Compiler.hpp"
#include "Diagnostic.hpp"
#include "Parser/this.hpp"
#include "Units/Package.hpp"
#include "Units/SourceFile.hpp"
#include "Sema/this.hpp"
#include <expected>
#include <filesystem>
#include <optional>

Compiler::~Compiler() {}

Compiler::Compiler() : m_packages(), m_sources() {
    m_typeRegistry.InitializeBuiltinTypes();
}

auto Compiler::OpenSource(this Compiler &self, std::string path)
    -> std::expected<SourceId, std::string> {
    if (!std::filesystem::exists(path)) {
        return std::unexpected("no such file");
    }

    auto canon = std::filesystem::canonical(path).string();

    if (auto file = self.m_sources.IdOf(canon); file.has_value()) {
        return SourceId(file.value());
    } else {
        SourceFile new_file{path};
        return SourceId(self.m_sources.Insert(canon, new_file));
    }
}

auto Compiler::NewPackage(this Compiler &self, std::string const &name,
                          SourceId of_root) -> PackageId {
    Package pkg{name, of_root};
    usize id = self.m_packages.size();
    self.m_packages.push_back(std::move(pkg));
    return PackageId(id);
}

auto Compiler::CompilePackage(this Compiler &self, PackageId pkgid,
                              DiagnosticPool &diagnostics) -> void {
    auto package = self.GetPackage(pkgid).value();
    if (auto source_file = self.GetSourceFile(package->GetRootSourceFile());
        source_file.has_value()) {
        auto *root_namespace_ = package->get_root();
        auto root_namespace = static_cast<Namespace *>(root_namespace_->get());
        Parser parser{pkgid, diagnostics, self, source_file.value().Content(),
                      package->GetRootSourceFile()};
        ParseIntoNamespace(parser, *root_namespace);

        SemanticAnalyzer sema{pkgid, self, diagnostics};
        sema.Run();
    } else {
        PANIC();
    }
}

auto Compiler::GetSourceFile(this Compiler &self, SourceId id)
    -> std::optional<SourceFile &> {
    return self.m_sources.GetFromId(id.id);
}

auto Compiler::GetSourceFile(this Compiler const &self, SourceId id)
    -> std::optional<SourceFile const &> {
    auto *source_file = self.m_sources.GetFromId(id.id);
    if (source_file != nullptr)
        return *source_file;
    return std::nullopt;
}

auto Compiler::GetPackage(this Compiler &self, PackageId id)
    -> std::optional<Package *> {
    return &self.m_packages[id.id];
}

auto Compiler::GetPackage(this Compiler const &self, PackageId id)
    -> std::optional<Package const *> {
    return &self.m_packages[id.id];
}

auto Compiler::GetRegistry(this Compiler &self) -> TypeRegistry & {
    return self.m_typeRegistry;
}

auto Compiler::GetRegistry(this Compiler const &self) -> TypeRegistry const & {
    return self.m_typeRegistry;
}