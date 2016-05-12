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
	binder(lts::builder &o, errors &e): out(o), err(e) {}
	virtual void emit(ast::type, std::string, location) override;
private:
	void ast_atom(ast::type, location);
	void ast_leaf(ast::type, std::string, location);
	void ast_branch(ast::type, std::string, location);
	lts::builder &out;
	errors &err;
};

#endif //BINDER_H

