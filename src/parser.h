// Copyright (C) 2016 Mars Saxman. All rights reserved.// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"
#include <stack>
#include <functional>

struct parser: public token::delegate {
	struct error {
		virtual void parser_unexpected(location) = 0;
		virtual void parser_missing_left_operand(location) = 0;
		virtual void parser_missing_right_operand(location) = 0;
		virtual void parser_expected(location, std::string, location) = 0;
	};
	parser(ast::processor &o, error &e): out(o), err(e) {}
	virtual void token_eof(location) override;
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
	virtual void token_colon_equal(location) override;
	virtual void token_double_colon_equal(location) override;
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
	virtual void token_tilde(location) override;
	virtual void token_bang(location) override;
	virtual void token_bang_equal(location) override;
	virtual void token_l_bangle(location) override;
	virtual void token_r_bangle(location) override;
	virtual void token_bang_tilde(location) override;
	virtual void token_ampersand(location) override;
	virtual void token_pipe(location) override;
	virtual void token_caret(location) override;
	virtual void token_l_guillemet(location) override;
	virtual void token_r_guillemet(location) override;
	virtual void token_l_arrow(location) override;
	virtual void token_r_arrow(location) override;
private:
	// the classic shunting-yard algorithm
	enum class precedence {
		structure, //R
		binding, //R
		relation, //L
		additive, //L
		multiplicative, //L
		negation, //R
		primary //L
	};
	struct oprec {
		precedence prec;
		location loc;
		std::function<void(void)> commit;
	};
	// Current state of the expression being parsed
	struct state {
		// infix operators in flight
		std::stack<oprec> ops;
		// most recent term or operator value
		ast::ptr exp;
		// other values awaiting operators
		std::stack<ast::ptr> vals;
	} context;
	// Outer context for a subexpression
	struct group {
		enum class delim { parens, brackets, braces };
		group(location l, delim t, state &&s):
				loc(l), type(t), context(std::move(s)) {}
		// location of the left-bracket used to enter this group
		location loc;
		// type of bracket we must match to close the group
		delim type;
		// the state as it existed before we entered the subexpression
		state context;
	};
	// Grouping stack outside the current expression context
	std::stack<group> outer;
	void open(location, group::delim);
	void close(group::delim, location);
	bool expecting_term();
	bool accept_term(location);
	bool accept_prefix(location);
	bool accept_infix(location);
	void emit(ast::node*);
	ast::ptr pop();
	ast::ptr cur();
	void term(location);
	void prefix(oprec);
	void infix(oprec);
	void commit_next();
	bool commit_all(location);
	ast::processor &out;
	error &err;
};

#endif //PARSER_H

