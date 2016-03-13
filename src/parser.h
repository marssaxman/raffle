// Copyright (C) 2016 Mars Saxman. All rights reserved.// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PARSER_H
#define PARSER_H

#include "errors.h"
#include "token.h"
#include "ast.h"
#include <stack>

struct parser: public token::ostream {
	parser(ast::builder &o, errors &e): out(o), err(e) {}

	// implementation of token::delegate
	virtual void flush() override;
	virtual void operator<<(token::number) override;
	virtual void operator<<(token::identifier) override;
	virtual void operator<<(token::string) override;
	virtual void operator<<(token::symbol) override;
	virtual void operator<<(token::delimiter) override;

private:
	// the classic shunting-yard algorithm
	enum class precedence {
		none = 0,
		sequence, //L
		binding, //R
		relation, //L
		additive, //L
		multiplicative, //L
		prefix, //R
		primary //L
	};

	// binary operators waiting for operands
	struct oprec {
		location loc;
		ast::branch id;
		precedence prec;
		std::string text;
	};
	std::stack<oprec> ops;
	bool expecting_term = true;

	// saved state for contexts outside the current expression
	struct context {
		location loc;
		std::string closer;
		std::stack<oprec> ops;
	};
	std::stack<context> outer;

	void reduce(precedence);
	void prep_term(location);
	precedence prep_operator(location, precedence);
	void push(oprec);
	void close(location);

	ast::builder &out;
	errors &err;
};

#endif //PARSER_H

