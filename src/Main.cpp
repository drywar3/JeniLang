#include "Compiler.hpp"
#include "Diagnostic.hpp"
#include "Units/Package.hpp"
#include <cstdio>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::printf("error: expected input file\n");
        return 1;
    }

    std::string input_file = argv[1];
    Compiler cc{};

    if (auto source_file_res = cc.OpenSource(input_file);
        source_file_res.has_value()) {
        DiagnosticPool diagnostic{1, 1};
        SourceId source_file = source_file_res.value();
        PackageId root       = cc.NewPackage("root", source_file);
        cc.CompilePackage(root, diagnostic);
    } else {
        std::string &err = source_file_res.error();
        std::printf("error: cannot open `%s`: %s\n", input_file.c_str(),
                    err.c_str());
        return 1;
    }
    return 0;
}
