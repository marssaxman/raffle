// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef RESOLVER_H
#define RESOLVER_H

#include "ast.h"
#include "dfg.h"

struct resolver: public ast::traversal {
	struct error {
		virtual void resolver_undefined(location) = 0;
		virtual void resolver_redefined(location, location) = 0;
	};
	resolver(ast::traversal &o, error &e): out(o), err(e) {}
	virtual void ast_open() override;
	virtual void ast_process(ast::ptr &&) override;
	virtual void ast_close() override;
private:
	ast::traversal &out;
	error &err;
};

#endif //RESOLVER_H

