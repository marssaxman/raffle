// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "resolver.h"
#include <stack>

namespace {
class rewriter: protected ast::visitor {
	ast::ptr out;
public:
	rewriter(ast::ptr &&o) {
		if (o) o->accept(*this);
		if (!out) out = std::move(o);
	}
	operator ast::ptr() {
		return std::move(out);
	}
};
}

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

namespace {
typedef std::map<std::string, size_t> symtab;
struct definer: public ast::visitor {
	definer(symtab &b, unsigned l): bindings(b), lambda_level(l) {}
	virtual void visit(ast::symbol &n) override {}
	symtab &bindings;
	unsigned lambda_level;
};
}

void resolver::visit(ast::symbol &n) {
	auto iter = bindings.find(n.text);
	if (iter == bindings.end()) {
		err.resolver_undefined(n.loc());
	}
	// The binding records the lambda nesting level of its container, from
	// the outside in. The parameter must record the context level of the
	// bound expression, from the inside out.
	unsigned index = lambda_level - iter->second;
	rewrite.reset(new ast::parameter(index, n.loc()));
}

void resolver::visit(ast::assign &n) {
	// assign ::= left <- right

}

void resolver::visit(ast::capture &n) {
	// capture ::= left -> right
	// Reduce to a lambda expression, substituting 'parameter' for 'left'
	// wherever it occurs inside 'right'.
	symtab nested(bindings);
	definer d(nested, lambda_level);
	n.left->accept(d);
//	binder b(nested, lambda_level);
//	n.right->accept(b);
//	rewrite.reset(new ast::lambda(
}

void resolver::process(ast::ptr &&tree) {
	if (!tree) return;
	tree->accept(*this);
	out.process(std::move(rewrite? rewrite: tree));
}

