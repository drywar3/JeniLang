#include "parser/parse_impl.hpp"
#include "entity/namespace.hpp"
#include "parser/parser.hpp"

auto parse_into_namespace(Parser &parser, Namespace *out) -> void {
    while (!parser.is_done()) {
        if (auto item = parse_item(parser); item.has_value()) {
            out->get_ast().push_back(std::move(item.value()));
        }
    }
}
