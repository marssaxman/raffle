// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "resolver.h"
#include <stack>

// Traverse an AST and resolve its symbol bindings.

namespace {

struct lookup: public ast::visitor {
	ast::ptr rewrite;
	virtual void visit(ast::symbol &n) override;
	virtual void visit(ast::assign &n) override;
	virtual void visit(ast::capture &n) override;
	virtual void visit(ast::define &n) override;
	virtual void visit(ast::typealias &n) override;
	virtual void visit(ast::sequence &n) override;
};
}

void lookup::visit(ast::symbol &n) {
}

void lookup::visit(ast::assign &n) {
}

void lookup::visit(ast::capture &n) {
}

void lookup::visit(ast::define &n) {
}

void lookup::visit(ast::typealias &n) {
}

void lookup::visit(ast::sequence &n) {
}

void resolver::ast_process(ast::ptr &&n) {
	out.ast_process(std::move(n));
}

