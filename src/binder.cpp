// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "binder.h"

void binder::ast_atom(location loc, ast::atom id) {
	switch (id) {
		case ast::atom::null: out.lts_null(loc); break;
		default: err.report(loc, "not yet implemented");
	}
}

void binder::ast_leaf(location loc, ast::leaf id, std::string t) {
	switch (id) {
		case ast::leaf::number: out.lts_number(t, loc); break;
		case ast::leaf::string: out.lts_string(t, loc); break;
		case ast::leaf::identifier: out.lts_symbol(t, loc); break;
	}
}

void binder::ast_branch(location loc, ast::branch id, std::string t) {
	switch (id) {
		case ast::branch::apply: out.lts_apply_top(loc); break;
		case ast::branch::pipe: out.lts_apply_next(loc); break;
		case ast::branch::capture: out.lts_lambda(loc); break;
		case ast::branch::sequence: out.lts_eval(loc); break;
		case ast::branch::assign: out.lts_bind(loc); break;
		case ast::branch::declare:
		case ast::branch::define:
		case ast::branch::typealias:
			err.report(loc, "operator '" + t + "' not yet implemented");
			break;
		default:
			out.lts_symbol(t, loc);
			out.lts_apply_next(loc);
			out.lts_apply_next(loc);
			break;
	}
}

