// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "resolver.h"

void resolver::ast_atom(location loc, ast::atom::tag id) {
	current_loc = loc;
	switch (id) {
		case ast::atom::wildcard: echo(); break;
		case ast::atom::null: null(); break;
	}
}

void resolver::ast_leaf(location loc, ast::leaf::tag id, std::string t) {
	current_loc = loc;
	switch (id) {
		case ast::leaf::number: number(t); break;
		case ast::leaf::string: string(t); break;
		case ast::leaf::symbol: param(); symbol(t); apply_LR(); break;
	}
}

void resolver::ast_branch(location loc, ast::branch::tag id, std::string t) {
	current_loc = loc;
	switch (id) {
		case ast::branch::apply: apply_LR(); break;
		case ast::branch::pipe: apply_RL(); break;
		case ast::branch::sequence: lambda_LR(); break;
		// the "extend" subscript operation would be lambda_RL()
		case ast::branch::assign:
			swap(); echo(); lambda_RL(); match_RL(); param(); join_LR();
			break;
		case ast::branch::capture:
			swap(); param(); match_LR(); apply_RL(); param(); lambda_RL();
			break;
		case ast::branch::declare:
		case ast::branch::define:
		case ast::branch::typealias:
			err.resolver_unimplemented(loc);
			break;
		default:
			pair_LR(); param(); symbol(t); apply_LR(); apply_RL();
			break;
	}
}

