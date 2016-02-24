// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "resolver.h"

void resolver::ast_atom(location loc, ast::atom::tag id) {
	switch (id) {
		case ast::atom::wildcard:
			// a function which has no context and returns any parameter value
			out.lts_atom(loc, lts::atom::param);
			out.lts_atom(loc, lts::atom::null);
			out.lts_branch(loc, lts::branch::lambda);
			break;
		case ast::atom::null:
			out.lts_atom(loc, lts::atom::null);
			break;
	}
}

void resolver::ast_leaf(location loc, ast::leaf::tag id, std::string t) {
	lts::leaf tag;
	switch (id) {
		case ast::leaf::number:
			out.lts_leaf(loc, lts::leaf::number, t);
			break;
		case ast::leaf::string:
			out.lts_leaf(loc, lts::leaf::string, t);
			break;
		case ast::leaf::symbol:
			// the context is a function which returns a value when given the
			// name of a variable; invoke it, supplying this variable symbol
			out.lts_leaf(loc, lts::leaf::symbol, t);
			out.lts_atom(loc, lts::atom::param);
			out.lts_branch(loc, lts::branch::apply);
			break;
	}
}

void resolver::ast_branch(location loc, ast::branch::tag id, std::string t) {
	switch (id) {
		case ast::branch::add:
		case ast::branch::sub:
		case ast::branch::mul:
		case ast::branch::div:
		case ast::branch::rem:
		case ast::branch::shl:
		case ast::branch::shr:
		case ast::branch::eq:
		case ast::branch::gt:
		case ast::branch::lt:
		case ast::branch::neq:
		case ast::branch::ngt:
		case ast::branch::nlt:
			out.lts_leaf(loc, lts::leaf::symbol, t);
			out.lts_atom(loc, lts::atom::param);
			out.lts_branch(loc, lts::branch::apply);
			out.lts_branch(loc, lts::branch::apply);
			out.lts_branch(loc, lts::branch::apply);
			break;
		default: err.resolver_unimplemented(loc);
	}
}

