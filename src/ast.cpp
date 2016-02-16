// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "ast.h"

using namespace ast;

subscript::subscript(opcode o, node &t, node &a, location l):
		id(o), target(t), argument(a), tk_loc(l) {
}

location subscript::loc() {
	return target.loc() + argument.loc() + tk_loc;
}

binary::binary(opcode o, node &l, node &r):
		id(o), left(l), right(r) {
}

location binary::loc() {
	return left.loc() + right.loc();
}

unary::unary(opcode o, node &s, location l):
		id(o), source(s), tk_loc(l) {
}

location unary::loc() {
	return tk_loc + source.loc();
}

