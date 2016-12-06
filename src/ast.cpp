// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "ast.h"

using namespace ast;

void eof::accept(visitor &v) const {
	v.visit(*this);
}

void wildcard::accept(visitor &v) const {
	v.visit(*this);
}

void null::accept(visitor &v) const {
	v.visit(*this);
}

void number::accept(visitor &v) const {
	v.visit(*this);
}

void string::accept(visitor &v) const {
	v.visit(*this);
}

void identifier::accept(visitor &v) const {
	v.visit(*this);
}

void apply::accept(visitor &v) const {
	v.visit(*this);
}

void pipe::accept(visitor &v) const {
	v.visit(*this);
}

void sequence::accept(visitor &v) const {
	v.visit(*this);
}

void pair::accept(visitor &v) const {
	v.visit(*this);
}

void range::accept(visitor &v) const {
	v.visit(*this);
}

void assign::accept(visitor &v) const {
	v.visit(*this);
}

void capture::accept(visitor &v) const {
	v.visit(*this);
}

void declare::accept(visitor &v) const {
	v.visit(*this);
}

void define::accept(visitor &v) const {
	v.visit(*this);
}

void typealias::accept(visitor &v) const {
	v.visit(*this);
}

void binop::accept(visitor &v) const {
	v.visit(*this);
}

