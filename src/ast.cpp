// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "ast.h"

// singleton null value used for optional expressions
ast::node ast::empty{};

using namespace ast;

literal::literal(opcode o, std::string t, location l):
		id(o), text(t), tk_loc(l) {
}

location literal::loc() {
	return tk_loc;
}

symbol::symbol(std::string t, location l):
		text(t), tk_loc(l) {
}

location symbol::loc() {
	return tk_loc;
}

unary::unary(opcode o, node &s, location l):
		id(o), source(s), tk_loc(l) {
}

location unary::loc() {
	return tk_loc + source.loc();
}

constructor::constructor(opcode o, node &i, location l):
		id(o), items(i), tk_loc(l) {
}

location constructor::loc() {
	// would be nice to capture the opening delimiter, too
	return items.loc() + tk_loc;
}

