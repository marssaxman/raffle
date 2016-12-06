// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef TREEGEN_H
#define TREEGEN_H

#include "ast.h"
#include "errors.h"
#include "syntax.h"
#include <stack>

struct treegen: public syntax::delegate {
	treegen(ast::delegate &o, errors &e): out(o), err(e) {}
	virtual void emit_eof(location) override;
	virtual void emit_wildcard(location) override;
	virtual void emit_null(location) override;
	virtual void emit_number(std::string, location) override;
	virtual void emit_string(std::string, location) override;
	virtual void emit_identifier(std::string, location) override;
	virtual void emit_branch(syntax::branch, std::string, location) override;
private:
	void store(ast::node*);
	ast::unique_ptr &&recall();
	std::stack<ast::unique_ptr> state;
	ast::delegate &out;
	errors &err;
};

#endif //TREEGEN_H
