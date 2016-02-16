// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "ast.h"

using namespace ast;

void empty::accept(visitor &v) {
	v.visit(*this);
}

literal::literal(opcode o, std::string t, location l):
		id(o), text(t), tk_loc(l) {
}

location literal::loc() {
	return tk_loc;
}

void literal::accept(visitor &v) {
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

void placeholder::accept(visitor &v) {
	v.visit(*this);
}

void invocation::accept(visitor &v) {
	v.visit(*this);
}

void definition::accept(visitor &v) {
	v.visit(*this);
}

void arithmetic::accept(visitor &v) {
	v.visit(*this);
}

void logic::accept(visitor &v) {
	v.visit(*this);
}

void relation::accept(visitor &v) {
	v.visit(*this);
}

void range::accept(visitor &v) {
	v.visit(*this);
}

void join::accept(visitor &v) {
	v.visit(*this);
}

void sequence::accept(visitor &v) {
	v.visit(*this);
}

void invert::accept(visitor &v) {
	v.visit(*this);
}

constructor::constructor(opcode o, node &i, location l):
		id(o), items(i), tk_loc(l) {
}

location constructor::loc() {
	// would be nice to capture the opening delimiter, too
	return items.loc() + tk_loc;
}

void constructor::accept(visitor &v) {
	v.visit(*this);
}

