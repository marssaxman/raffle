// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef HOAS_H
#define HOAS_H

#include "ast.h"

namespace hoas {
struct visitor;

struct lambda: public ast::unary {
	lambda(ast::ptr &&e, location l): unary(std::move(e)), tk_loc(l) {}
	virtual void accept(visitor &v);
	virtual location loc() override { return tk_loc + source->loc(); }
private:
	location tk_loc;
};

struct parameter: public ast::node {
	parameter(unsigned i, location l): index(i), tk_loc(l) {}
	unsigned index = 0; // de bruijn
	virtual void accept(visitor &v);
	virtual location loc() override { return tk_loc; }
private:
	location tk_loc;
};

struct visitor: public ast::visitor {
	virtual void visit(lambda &n) { ast::visitor::visit((ast::unary&)n); }
	virtual void visit(parameter &n) { ast::visitor::visit((ast::node&)n); }
};
}

struct traversal: public ast::traversal {
	virtual void hoas_process(ast::ptr &&n) { ast_process(std::move(n)); }
};

#endif //HOAS_H

