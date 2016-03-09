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
	virtual void ast_atom(location, ast::atom) override;
	virtual void ast_leaf(location, ast::leaf, std::string) override;
	virtual void ast_branch(location, ast::branch, std::string) override;
private:
	lts::builder &out;
	errors &err;
};

#endif //BINDER_H

