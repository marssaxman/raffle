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
	parser(ast::delegate &o, errors &e): out(o), err(e) {}

	// implementation of token::delegate
	virtual void token_eof(location) override;
	virtual void token_number(location, std::string) override;
	virtual void token_identifier(location, std::string) override;
	virtual void token_string(location, std::string) override;
	virtual void token_underscore(location) override;
	virtual void token_open(location, token::delim) override;
	virtual void token_close(location, token::delim) override;
	virtual void token_symbol(location, std::string) override;

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
		token::delim type;
		std::stack<oprec> ops;
	};
	std::stack<context> outer;

	// common state management for various types of tokens
	void reduce(precedence);
	void prep_term(location);
	void prep_operator(location);
	void push(oprec);
	void close(location);

	// Client that actually builds the AST we specify
	ast::delegate &out;
	errors &err;
};

#endif //PARSER_H

