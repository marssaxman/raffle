// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "resolver.h"

// ok so the glorious truth is that this process completely inverts the tree
// we start out with this:
//    ( foo := bar ) ; baz[foo]
// and we build something like this:
//    ( $ -> baz[$] ) bar
// where $ is an unambiguous reference to the lambda parameter.
// this is exactly the same thing we do with an assignment, too:
//    ( foo <- bar ) ; baz[foo]
// would become
//    ( $ -> baz[$] ) bar
// Right, yeah. Exactly the same, then, it's just OK if the symbol already had
// a previous binding, which we will replace.
// So perhaps we actually look at it as two transformation stages:
//  - convert definitions and assignments into capture expressions
//  - bind capture expression parameters, yielding lambda nodes

// wow. so assignment really is an exact inversion of capture, and the arrows
// really are exact opposites: the right arrow captures inwards, defining the
// RHS expression as a function of the named parameter, while the left arrow
// captures outwards, defining *the whole tree containing the assignment* as
// a function of the named parameter!
// However deep we are in the nesting, that's what's going on: everything
// downstream from the assignment is a function of some unknown value, which
// will be applied by an outer context, where that "outer" context happens to
// be the value specified on the right side of the left arrow.

// So: there we go. First job, turn the tree inside out. Second job, match all
// the names up with symbols.

void resolver::visit(ast::capture &n) {
	// capture ::= left -> right
	// Reduce to a lambda expression, substituting 'parameter' for 'left'
	// wherever it occurs inside 'right'.
	
}

