// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "resolver.h"

void resolver::ast_atom(location loc, ast::atom id) {
	current_loc = loc;
	switch (id) {
		case ast::atom::wildcard:
			echo();
			break;
		case ast::atom::null:
			null();
			break;
	}
}

void resolver::ast_leaf(location loc, ast::leaf id, std::string t) {
	current_loc = loc;
	switch (id) {
		case ast::leaf::number:
			number(t);
			break;
		case ast::leaf::string:
			string(t);
			break;
		case ast::leaf::symbol:
			env();
			symbol(t);
			apply_LR();
			break;
	}
}

void resolver::ast_branch(location loc, ast::branch id, std::string t) {
	current_loc = loc;
	switch (id) {
		case ast::branch::apply:
			apply_LR();
			break;
		case ast::branch::pipe:
			apply_RL();
			break;
		case ast::branch::sequence:
			lambda_LR();
			param();
			apply_RL();
			break;
		case ast::branch::assign:
			swap();
			echo();
			lambda_RL();
			match_RL();
			env();
			join_LR();
			break;
		case ast::branch::define:
			swap();
			echo();
			lambda_RL();
			match_RL();
			env();
			join_RL();
			break;
		case ast::branch::capture:
			param();
			swap();
			echo();
			lambda_RL();
			match_RL();
			env();
			join_RL();
			param();
			apply_RL();
			env();
			lambda_RL();
			break;
		case ast::branch::declare:
		case ast::branch::typealias:
			err.resolver_unimplemented(loc);
			break;
		default:
			pair_LR();
			env();
			symbol(t);
			apply_LR();
			apply_RL();
			break;
	}
}

