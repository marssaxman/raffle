// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "resolver.h"

using namespace ast;
namespace {

struct index {
	void declare(symbol &name, node &type) {}
	void define(symbol &name, node &value) {}
	void typealias(symbol &name, node &type) {}
	bool lookup(symbol &name) {}
};

struct context: public visitor {
	typedef resolver::error error;
	context(index &s, error &e): symbols(s), err(e) {}
protected:
	index &symbols;
	error &err;
};

// LHS of a variable, constant, or type definition expression
struct definition: public context {
	using context::context;
	virtual void visit(node&) override;
	virtual void visit(symbol &n) override;
};

// LHS of an assignment or capture expression
struct target: public context {
	using context::context;
	virtual void visit(node&) override;
	virtual void visit(symbol &n) override;
	virtual void visit(wildcard &n) override {}
};

// RHS of a variable, type declaration, or type comparison expression
struct constraint: public context {
	using context::context;
	virtual void visit(node&) override;
	virtual void visit(symbol &n) override;
	virtual void visit(wildcard &n) override {}
};

// The normal case, where expressions are evaluated to produce a value
struct evaluate: public context {
	using context::context;
	virtual void visit(binary &n) override;
	virtual void visit(symbol &n) override;
	virtual void visit(wildcard &n) override;
	virtual void visit(assign &n) override;
	virtual void visit(capture &n) override;
	virtual void visit(declare &n) override;
	virtual void visit(define &n) override;
	virtual void visit(typealias &n) override;
	virtual void visit(group &n) override;
};

} // namespace

void definition::visit(node &n) {
	err.resolver_unexpected_definition(n.loc());
}

void definition::visit(symbol &n) {
	// The symbol must not already have been defined in the current scope.
	// This will become its definition.
}

void target::visit(node &n) {
	err.resolver_unexpected_target(n.loc());
}

void target::visit(symbol &n) {
	// If this symbol has already been defined, it must be a variable, not a
	// permanent binding. If it has not been defined, define it implicitly.
}

void constraint::visit(node &n) {
	err.resolver_unexpected_constraint(n.loc());
}

void constraint::visit(symbol &n) {
	// This symbol must have been defined, and it must have been defined as a
	// type name.
}

void evaluate::visit(binary &n) {
	n.left->accept(*this);
	n.right->accept(*this);
}

void evaluate::visit(symbol &n) {
	// The symbol must have already been defined.
}

void evaluate::visit(wildcard &n) {
	err.resolver_unexpected_wildcard(n.loc());
}

void evaluate::visit(assign &n) {
	// Evaluate the right, then target the left in the current scope.
}

void evaluate::visit(capture &n) {
	// In a subsidiary scope, target the left, then evaluate the right.
}

void evaluate::visit(declare &n) {
	// RHS is a type constraint, LHS is a variable definition.
}

void evaluate::visit(define &n) {
	// RHS is a value expression, LHS is a binding definition.
}

void evaluate::visit(typealias &n) {
	// RHS is a type constraint, LHS is a binding definition.
}

void evaluate::visit(group &n) {
	for (auto &i: n.items) {
		i->accept(*this);
	}
}

void resolver::ast_open(ast::group &n) {
	out.ast_open(n);
}

void resolver::ast_process(ast::node &n) {
	out.ast_process(n);
}

void resolver::ast_close(ast::group &n) {
	out.ast_close(n);
}


