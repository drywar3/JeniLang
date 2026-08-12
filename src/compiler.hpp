#pragma once

#include "common/idmap.hpp"
#include "diagnostic.hpp"
#include "units/package.hpp"
#include "units/source_file.hpp"
#include <expected>
#include <optional>
#include <vector>

class Compiler {
  public:
    Compiler();
    ~Compiler();

    PackageId new_package(this Compiler &, std::string const &name,
                          SourceId of_root);

    auto open_source(this Compiler &, std::string path)
        -> std::expected<SourceId, std::string>;
    auto compile_package(this Compiler &, PackageId, DiagnosticPool &) -> void;

    auto get_source_file(this Compiler &, SourceId id)
        -> std::optional<SourceFile &>;
    auto get_source_file(this Compiler const &, SourceId id)
        -> std::optional<SourceFile const &>;

    auto get_package(this Compiler &, PackageId id) -> std::optional<Package *>;
    auto get_package(this Compiler const &, PackageId id)
        -> std::optional<Package const *>;

  private:
    std::vector<Package> m_packages;
    IdMap<std::string, SourceFile> m_sources;
};
