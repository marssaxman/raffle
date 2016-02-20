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

void apply::accept(visitor &v) {
	v.visit(*this);
}

void pipeline::accept(visitor &v) {
	v.visit(*this);
}

void assign::accept(visitor &v) {
	v.visit(*this);
}

void capture::accept(visitor &v) {
	v.visit(*this);
}

void declare::accept(visitor &v) {
	v.visit(*this);
}

void define::accept(visitor &v) {
	v.visit(*this);
}

void typealias::accept(visitor &v) {
	v.visit(*this);
}

binary::binary(ptr &&l, ptr &&r):
		left(std::move(l)), right(std::move(r)) {
	assert(left);
	assert(right);
}

operate::operate(opcode o, ptr &&l, ptr &&r):
		binary(std::move(l), std::move(r)), id(o) {
}

void operate::accept(visitor &v) {
	v.visit(*this);
}

void range::accept(visitor &v) {
	v.visit(*this);
}

negate::negate(opcode o, ptr &&s, location l):
		id(o), source(std::move(s)), tk_loc(l) {
}

void negate::accept(visitor &v) {
	v.visit(*this);
}

group::group(opcode o, std::list<ptr> &&i, location l, location r):
		id(o), items(std::move(i)), open_loc(l), close_loc(r) {
}

void group::accept(visitor &v) {
	v.visit(*this);
}

