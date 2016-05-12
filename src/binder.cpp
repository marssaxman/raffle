// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "binder.h"

void binder::emit(ast::type id, std::string text, location loc) {
	switch (ast::edges(id)) {
		case 0: ast_atom(id, loc); break;
		case 1: ast_leaf(id, text, loc); break;
		case 2: ast_branch(id, text, loc); break;
		default: err.report(loc, "unexpected AST edge count");
	}
}

void binder::ast_atom(ast::type id, location loc) {
	switch (id) {
		case ast::null: out.lts_null(loc); break;
		default: err.report(loc, "not yet implemented");
	}
}

void binder::ast_leaf(ast::type id, std::string t, location loc) {
	switch (id) {
		case ast::number: out.lts_number(t, loc); break;
		case ast::string: out.lts_string(t, loc); break;
		case ast::identifier: out.lts_symbol(t, loc); break;
		default: err.report(loc, "unexpected leaf node");
	}
}

void binder::ast_branch(ast::type id, std::string t, location loc) {
	switch (id) {
		case ast::apply: out.lts_apply_top(loc); break;
		case ast::pipe: out.lts_apply_next(loc); break;
		case ast::capture: out.lts_lambda(loc); break;
		case ast::sequence: out.lts_eval(loc); break;
		case ast::assign: out.lts_bind(loc); break;
		case ast::declare:
		case ast::define:
		case ast::typealias:
			err.report(loc, "operator '" + t + "' not yet implemented");
			break;
		default:
			out.lts_symbol(t, loc);
			out.lts_apply_next(loc);
			out.lts_apply_next(loc);
			break;
	}
}

