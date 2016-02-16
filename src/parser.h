// Copyright (C) 2016 Mars Saxman. All rights reserved.// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "syntax.h"
#include <stack>
#include <queue>

class parser: public token::delegate {
	typedef token::direction direction;
	// the classic shunting-yard algorithm
	// operator precedence encoded in high nibble
	// even precedence levels associate left, odd levels associate right
	enum class op: int {
		// bracketed groups
		eval = 0x00, list, object, subscript,
		// statements
		sequence = 0x10,
		// symbols
		capture = 0x30, define,
		// structure
		join = 0x50, caption,
		// relation
		equal = 0x60, lesser, greater, not_equal, not_lesser, not_greater,
		// additive
		add = 0x80, subtract, disjoin, exclude, range,
		// multiplicative
		multiply = 0xA0, divide, modulo, shift_left, shift_right, conjoin,
		// unary
		negate = 0xB0, complement,
		// primary
		lookup = 0xC0,
	};
	struct oprec {
		op id;
		location loc;
	};
	std::stack<oprec> ops;
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
	parser(syntax::delegate &o, error &e): out(o), err(e) {}
	virtual void token_number(location, std::string) override;
	virtual void token_symbol(location, std::string) override;
	virtual void token_string(location, std::string) override;
	virtual void token_underscore(location) override;
	virtual void token_paren(location, direction) override;
	virtual void token_bracket(location, direction) override;
	virtual void token_brace(location, direction) override;
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
	virtual void token_angle(location, direction) override;
	virtual void token_bang(location) override;
	virtual void token_bang_equal(location) override;
	virtual void token_bangle(location, direction) override;
	virtual void token_ampersand(location) override;
	virtual void token_pipe(location) override;
	virtual void token_caret(location) override;
	virtual void token_guillemet(location, direction) override;
	virtual void token_arrow(location, direction) override;
	void flush();
protected:
	static int precedence(op x);
	static bool rightassoc(op x);
	typedef void (syntax::delegate::*leaf_rule)(location, std::string);
	typedef void (syntax::delegate::*tree_rule)(location);
	void term(leaf_rule, location, std::string);
	void group(op, tree_rule term, tree_rule sub, location, direction);
	void open_group(op, location);
	void close_group(op, tree_rule term, tree_rule sub, location);
	void directional(op l, op r, location, direction);
	void unary(op, location);
	void binary(op, location);
	void commit();
private:
	syntax::delegate &out;
	error &err;
};

#endif //PARSER_H

