#pragma once

#include "ast/item.hpp"
#include <vector>

using Ast = std::vector<ItemRef>;

struct AstId {
public:
	explicit AstId(usize id)
		: id(id) {}

	usize const id;
};
