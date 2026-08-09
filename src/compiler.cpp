#include "compiler.hpp"
#include "diagnostic.hpp"
#include "parser/parser.hpp"
#include "units/package.hpp"
#include "units/source_file.hpp"
#include <expected>
#include <filesystem>

Compiler::~Compiler() {}

Compiler::Compiler() : m_packages(), m_sources() {}

auto Compiler::open_source(this Compiler &self, std::string path)
    -> std::expected<SourceId, std::string> {
    if (!std::filesystem::exists(path)) {
        return std::unexpected("no such file");
    }

    auto canon = std::filesystem::canonical(path).string();

    if (auto file = self.m_sources.id_of(canon); file.has_value()) {
        return SourceId(file.value());
    } else {
        SourceFile new_file{path};
        return SourceId(self.m_sources.insert(canon, new_file));
    }
}

auto Compiler::new_package(this Compiler &self, std::string const &name,
                           SourceId of_root) -> PackageId {
    Package pkg{name, of_root};
    usize id = self.m_packages.size();
    self.m_packages.push_back(std::move(pkg));
    return PackageId(id);
}

auto Compiler::compile_package(this Compiler &self, PackageId pkgid,
                               DiagnosticPool &diagnostics) -> void {
    auto package = self.get_package(pkgid);
    if (auto source_file =
            self.get_source_file(package->get_root_source_file());
        source_file.has_value()) {
        auto root_namespace_ = package->get_symbol(package->get_root());
        auto root_namespace =
            static_cast<Namespace *>(root_namespace_.value().get());
        Parser parser{diagnostics, self, source_file.value().content(),
                      package->get_root_source_file()};
        parse_into_namespace(parser, root_namespace);
    } else {
        PANIC();
    }
}

auto Compiler::get_source_file(this Compiler &self, SourceId id)
    -> std::optional<SourceFile &> {
    return self.m_sources.get_from_id(id.id);
}

auto Compiler::get_source_file(this Compiler const &self, SourceId id)
    -> std::optional<SourceFile const &> {
    return self.m_sources.get_from_id(id.id);
}

auto Compiler::get_package(this Compiler &self, PackageId id)
    -> std::optional<Package &> {
    return self.m_packages[id.id];
}

auto Compiler::get_package(this Compiler const &self, PackageId id)
    -> std::optional<Package const &> {
    return self.m_packages[id.id];
}
