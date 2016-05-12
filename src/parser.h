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

struct parser: public token::delegate {
	parser(ast::builder &o, errors &e): out(o), err(e) {}

	// implementation of token::delegate
	virtual void parse(token::type, std::string, location) override;
	virtual void flush() override;

private:
	void parse_number(std::string, location);
	void parse_identifier(std::string, location);
	void parse_string(std::string, location);
	void parse_symbol(std::string, location);
	void parse_delimiter(std::string, location);

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

