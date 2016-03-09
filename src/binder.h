// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef BINDER_H
#define BINDER_H

#include "errors.h"
#include "ast.h"
#include "lts.h"

struct binder: public ast::builder {
	binder(lts::delegate &o, errors &e): out(o), err(e) {}
	virtual void ast_atom(location, ast::atom) override;
	virtual void ast_leaf(location, ast::leaf, std::string) override;
	virtual void ast_branch(location, ast::branch, std::string) override;
private:
	void swap() { out.lts_swap(); }
	void param() { out.lts_atom(current_loc, lts::atom::param); }
	void env() { out.lts_atom(current_loc, lts::atom::env); }
	void null() { out.lts_atom(current_loc, lts::atom::null); }
	void echo() { out.lts_atom(current_loc, lts::atom::echo); }
	void leaf(lts::leaf i, std::string t) { out.lts_leaf(current_loc, i, t); }
	void number(std::string t) { leaf(lts::leaf::number, t); }
	void string(std::string t) { leaf(lts::leaf::string, t); }
	void symbol(std::string t) { leaf(lts::leaf::symbol, t); }
	void apply_LR() { out.lts_branch(current_loc, lts::branch::apply); }
	void apply_RL() { swap(); apply_LR(); }
	void lambda_LR() { out.lts_branch(current_loc, lts::branch::lambda); }
	void lambda_RL() { swap(); lambda_LR(); }
	void pair_LR() { out.lts_branch(current_loc, lts::branch::pair); }
	void pair_RL() { swap(); pair_LR(); }
	void match_LR() { out.lts_branch(current_loc, lts::branch::match); }
	void match_RL() { swap(); match_LR(); }
	void join_LR() { out.lts_branch(current_loc, lts::branch::join); }
	void join_RL() { swap(); join_LR(); }
	location current_loc;
	lts::delegate &out;
	errors &err;
};

#endif //BINDER_H

