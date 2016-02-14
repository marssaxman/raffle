// Copyright (C) 2016 Mars Saxman. All rights reserved.// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stack>
#include <queue>

class parser: public lexer::output {
	// the classic shunting-yard algorithm
	struct token {
		int id;
		location loc;
	};
	std::stack<int> values;
	std::stack<token> ops;
	// availability of operators depends on context
	enum class state {
		empty,	// initial state, no value yet
		value,	// last token completed an expression value
		prefix,	// after unary operator, expecting an operand
		infix,	// after binary operator, expecting an operand
	} context = state::empty;
public:
	struct output {
		virtual int rule_empty() = 0; // for optional items
		virtual int rule_number(std::string) = 0;
		virtual int rule_symbol(std::string) = 0;
		virtual int rule_string(std::string) = 0;
		virtual int rule_placeholder() = 0; // 
		virtual int rule_sequence(int, int) = 0;
		virtual int rule_capture(int, int) = 0;
		virtual int rule_define(int, int) = 0;
		virtual int rule_join(int, int) = 0;
		virtual int rule_caption(int, int) = 0;
		virtual int rule_equal(int, int) = 0;
		virtual int rule_lesser(int, int) = 0;
		virtual int rule_greater(int, int) = 0;
		virtual int rule_addition(int, int) = 0;
		virtual int rule_subtraction(int, int) = 0;
		virtual int rule_or(int, int) = 0;
		virtual int rule_xor(int, int) = 0;
		virtual int rule_range(int, int) = 0;
		virtual int rule_multiplication(int, int) = 0;
		virtual int rule_division(int, int) = 0;
		virtual int rule_modulo(int, int) = 0;
		virtual int rule_shift_left(int, int) = 0;
		virtual int rule_shift_right(int, int) = 0;
		virtual int rule_and(int, int) = 0;
		virtual int rule_negate(int) = 0;
		virtual int rule_complement(int) = 0;
		virtual int rule_eval(int) = 0;
		virtual int rule_list(int) = 0;
		virtual int rule_object(int) = 0;
		virtual int rule_subscript(int, int) = 0;
		virtual int rule_lookup(int, int) = 0;
	};
	struct error {
		virtual void parser_unexpected(location) = 0;
		virtual void parser_missing_operand(location) = 0;
		virtual void parser_mismatched_group(location) = 0;
		virtual void parser_unimplemented(location) = 0;
	};
	parser(output &o, error &e): out(o), err(e) {}
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
	virtual void token_ampersand(location) override;
	virtual void token_pipe(location) override;
	virtual void token_caret(location) override;
	virtual void token_guillemet(location, direction) override;
	virtual void token_arrow(location, direction) override;
	void flush();
protected:
	void accept(int val);
	int recall();
	void term(int (output::*rule)(std::string), location, std::string);
	void group(int tk, int (output::*rule)(int), location, direction);
	void open_group(int tk, location);
	void close_group(int tk, int (output::*rule)(int), location);
	void directional(int l, int r, location, direction);
	void unary(int, location);
	void binary(int, location);
	void commit();
private:
	output &out;
	error &err;
};

#endif //PARSER_H

