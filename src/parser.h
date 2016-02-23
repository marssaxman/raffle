// Copyright (C) 2016 Mars Saxman. All rights reserved.// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"
#include <stack>

struct parser: public token::delegate {
	struct error {
		virtual void parser_unexpected(location) = 0;
		virtual void parser_missing_left_operand(location) = 0;
		virtual void parser_missing_right_operand(location) = 0;
		virtual void parser_expected(location, std::string, location) = 0;
	};
	parser(ast::builder &o, error &e): out(o), err(e) {}
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
		structure, //L
		binding, //R
		relation, //L
		additive, //L
		multiplicative, //L
		prefix, //R
		primary //L
	};

	// binary operators waiting for operands
	struct oprec {
		ast::branch::tag id;
		precedence prec;
		location loc;
	};
	std::stack<oprec> ops;
	bool expecting_term = true;

	// saved state for contexts outside the current expression
	struct context {
		token::delim type;
		location loc;
		std::stack<oprec> ops;
	};
	std::stack<context> outer;

	// common state management for various types of tokens
	void commit();
	void reduce(precedence);

	// Client that actually builds the AST we specify
	ast::builder &out;
	error &err;
};

#endif //PARSER_H

