// Copyright (C) 2016 Mars Saxman. All rights reserved.// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"
#include <stack>
#include <queue>
#include <functional>

class parser: public token::delegate {
	// the classic shunting-yard algorithm
	enum class precedence {
		statement, //R
		definition, //R
		structure, //R
		relation, //L
		additive, //L
		multiplicative, //L
		unary, //R
		primary //L
	};
	static bool rightassoc(precedence);
	struct oprec {
		precedence prec;
		location loc;
		std::function<void(void)> commit;
	};
	std::stack<oprec> ops;
	std::queue<ast::ptr> vals;
	// availability of operators depends on context
	enum class state {
		empty,	// initial state, no value yet
		value,	// last token completed an expression value
		prefix,	// after unary operator, expecting an operand
		infix,	// after binary operator, expecting an operand
	} context = state::empty;
public:
	struct error {
		virtual void parser_unexpected(location) = 0;
		virtual void parser_missing_operand(location) = 0;
		virtual void parser_mismatched_group(location) = 0;
		virtual void parser_unimplemented(location) = 0;
	};
	parser(error &e): err(e) {}
	virtual void token_number(location, std::string) override;
	virtual void token_symbol(location, std::string) override;
	virtual void token_string(location, std::string) override;
	virtual void token_underscore(location) override;
	virtual void token_l_paren(location) override;
	virtual void token_r_paren(location) override;
	virtual void token_l_bracket(location) override;
	virtual void token_r_bracket(location) override;
	virtual void token_l_brace(location) override;
	virtual void token_r_brace(location) override;
	virtual void token_comma(location) override;
	virtual void token_colon(location) override;
	virtual void token_semicolon(location) override;
	virtual void token_dot(location) override;
	virtual void token_dot_dot(location) override;
	virtual void token_plus(location) override;
	virtual void token_hyphen(location) override;
	virtual void token_star(location) override;
	virtual void token_slash(location) override;
	virtual void token_percent(location) override;
	virtual void token_equal(location) override;
	virtual void token_l_angle(location) override;
	virtual void token_r_angle(location) override;
	virtual void token_bang(location) override;
	virtual void token_bang_equal(location) override;
	virtual void token_l_bangle(location) override;
	virtual void token_r_bangle(location) override;
	virtual void token_ampersand(location) override;
	virtual void token_pipe(location) override;
	virtual void token_caret(location) override;
	virtual void token_l_guillemet(location) override;
	virtual void token_r_guillemet(location) override;
	virtual void token_l_arrow(location) override;
	virtual void token_r_arrow(location) override;
	void flush();
private:
	void push(ast::node*);
	ast::ptr &&pop();
	void prefix(oprec);
	void infix(oprec);
	void commit();
	error &err;
};

#endif //PARSER_H

