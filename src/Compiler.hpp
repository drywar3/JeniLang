#pragma once

#include "Common/IdMap.hpp"
#include "Diagnostic.hpp"
#include "Units/Package.hpp"
#include "Units/SourceFile.hpp"
#include <expected>
#include <optional>
#include <vector>

class Compiler {
  public:
    Compiler();
    ~Compiler();

    PackageId NewPackage(this Compiler &, std::string const &name,
                          SourceId of_root);

    auto OpenSource(this Compiler &, std::string path)
        -> std::expected<SourceId, std::string>;
    auto CompilePackage(this Compiler &, PackageId, DiagnosticPool &) -> void;

    auto GetSourceFile(this Compiler &, SourceId id)
        -> std::optional<SourceFile &>;
    auto GetSourceFile(this Compiler const &, SourceId id)
        -> std::optional<SourceFile const &>;

    auto GetPackage(this Compiler &, PackageId id) -> std::optional<Package *>;
    auto GetPackage(this Compiler const &, PackageId id)
        -> std::optional<Package const *>;

  private:
    std::vector<Package> m_packages;
    IdMap<std::string, SourceFile> m_sources;
};
