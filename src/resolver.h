// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef RESOLVER_H
#define RESOLVER_H

#include "ast.h"
#include "lts.h"

struct resolver: public ast::delegate {
	struct error {
		virtual void resolver_unimplemented(location) = 0;
	};
	resolver(lts::delegate &o, error &e): out(o), err(e) {}
	virtual void ast_atom(location, ast::atom::tag) override;
	virtual void ast_leaf(location, ast::leaf::tag, std::string) override;
	virtual void ast_branch(location, ast::branch::tag, std::string) override;
private:
	lts::delegate &out;
	error &err;
};

#endif //RESOLVER_H

