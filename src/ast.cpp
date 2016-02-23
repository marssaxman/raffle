// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "ast.h"
#include <assert.h>

using namespace ast;

void leaf::accept(visitor &v) {
	v.visit(*this);
}

branch::branch(tag o, ptr &&l, ptr &&r):
		id(o), left(std::move(l)), right(std::move(r)) {
	assert(left);
	assert(right);
}

void branch::accept(visitor &v) {
	v.visit(*this);
}


