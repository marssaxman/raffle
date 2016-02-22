// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "ast.h"
#include <assert.h>

using namespace ast;

void number::accept(visitor &v) {
	v.visit(*this);
}

void string::accept(visitor &v) {
	v.visit(*this);
}

symbol::symbol(std::string t, location l):
		text(t), tk_loc(l) {
}

location symbol::loc() {
	return tk_loc;
}

void symbol::accept(visitor &v) {
	v.visit(*this);
}

void wildcard::accept(visitor &v) {
	v.visit(*this);
}

group::group(location l, ptr &&s, location r):
	source(std::move(s)), openloc(l), closeloc(r) {
}

void parens::accept(visitor &v) {
	v.visit(*this);
}

void brackets::accept(visitor &v) {
	v.visit(*this);
}

void braces::accept(visitor &v) {
	v.visit(*this);
}

binary::binary(opcode o, ptr &&l, ptr &&r):
		id(o), left(std::move(l)), right(std::move(r)) {
	assert(left);
	assert(right);
}

void binary::accept(visitor &v) {
	v.visit(*this);
}

ostream &processor::operator<<(ptr &&n) {
	if (n) n->accept(*this);
	out << std::move(replace? replace: n);
	return *this;
}

void processor::send(ptr &&n) {
	if (replace) out << std::move(replace);
	replace = std::move(n);
}

