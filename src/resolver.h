// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef RESOLVER_H
#define RESOLVER_H

#include "ast.h"
#include <map>

// The resolver receives an AST, matches symbol references with definitions,
// and produces a higher-order AST, a "lambda syntax tree", where bindings are
// represented by lambda capture and function application.
struct resolver: public ast::ostream, private ast::visitor {
	struct error {
		virtual void resolver_undefined(location) = 0;
		virtual void resolver_redefined(location, location) = 0;
		virtual void resolver_unexpected_definition(location) = 0;
		virtual void resolver_unexpected_target(location) = 0;
		virtual void resolver_unexpected_constraint(location) = 0;
		virtual void resolver_unexpected_wildcard(location) = 0;
	};
	resolver(ast::ostream &o, error &e): out(o), err(e) {}
	virtual ast::ostream &operator<<(ast::ptr&&) override;
private:
	virtual void visit(ast::symbol&) override;
	virtual void visit(ast::assign&) override;
	virtual void visit(ast::capture&) override;
	unsigned lambda_level = 0;
	std::map<std::string, size_t> bindings;
	ast::ptr rewrite = nullptr;
	ast::ostream &out;
	error &err;
};

#endif //RESOLVER_H

