// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "resolver.h"

// nodes the resolver might care about:
// symbol: meaning depends on context
// assign: binds a new value to a symbol in place of the old one
// capture: LHS declared as abstract inside RHS
// define: RHS evaluated and bound to LHS
// typealias: RHS evaluated and bound to LHS
// sequence: evaluate LHS, then RHS, returning only the value of RHS
// tuple: evaluate LHS, then RHS, returning both values

// The resolver looks for these patterns:
//   foo := bar; baz
//   foo <- bar; baz
// It converts them into a pair of higher-order operations, like this:
//   (_ -> baz)(bar)
// That is, we convert "baz" into a function of some anonymous parameter, where
// all uses of "foo" become references to the parameter, followed by a call to
// that anonymous function which supplies the value.

void resolver::ast_process(ast::ptr &&n) {
	out.ast_process(std::move(n));
}


