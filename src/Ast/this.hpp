#pragma once

#include "Ast/Item.hpp"
#include <vector>

using Ast = std::vector<ItemRef>;

struct AstId {
public:
	explicit AstId(usize id)
		: id(id) {}

	usize const id;
};
