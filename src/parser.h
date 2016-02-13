// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
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
	std::stack<int> values;
	std::stack<int> states;
	// some tokens apply either infix or prefix
	bool prefix = true;
public:
	struct output {
		virtual int parse_number(std::string) = 0;
		virtual int parse_symbol(std::string) = 0;
		virtual int parse_string(std::string) = 0;
		virtual int parse_paren_empty() = 0;
		virtual int parse_bracket_empty() = 0;
		virtual int parse_brace_empty() = 0;
		virtual int parse_sequence(int, int) = 0;
		virtual int parse_capture(int, int) = 0;
		virtual int parse_define(int, int) = 0;
		virtual int parse_list(int, int) = 0;
		virtual int parse_caption(int, int) = 0;
		virtual int parse_equal(int, int) = 0;
		virtual int parse_lesser(int, int) = 0;
		virtual int parse_greater(int, int) = 0;
		virtual int parse_addition(int, int) = 0;
		virtual int parse_subtraction(int, int) = 0;
		virtual int parse_or(int, int) = 0;
		virtual int parse_xor(int, int) = 0;
		virtual int parse_range(int, int) = 0;
		virtual int parse_multiplication(int, int) = 0;
		virtual int parse_division(int, int) = 0;
		virtual int parse_modulo(int, int) = 0;
		virtual int parse_shift_left(int, int) = 0;
		virtual int parse_shift_right(int, int) = 0;
		virtual int parse_and(int, int) = 0;
		virtual int parse_negate(int) = 0;
		virtual int parse_paren_group(int) = 0;
		virtual int parse_bracket_group(int) = 0;
		virtual int parse_brace_group(int) = 0;
		virtual int parse_subscript(int, int) = 0;
	};
	struct error {
		virtual void parse_unexpected(lexer::position) = 0;
		virtual void parse_mismatched_paren(lexer::position) = 0;
		virtual void parse_mismatched_bracket(lexer::position) = 0;
		virtual void parse_mismatched_brace(lexer::position) = 0;
	};
	parser(output &o, error &e): out(o), err(e) {}
	virtual void token_number(lexer::position p, std::string text) override;
	virtual void token_symbol(lexer::position p, std::string text) override;
	virtual void token_string(lexer::position p, std::string text) override;
	virtual void token_paren_empty(lexer::position) override;
	virtual void token_paren_open(lexer::position) override;
	virtual void token_paren_close(lexer::position) override;
	virtual void token_bracket_empty(lexer::position) override;
	virtual void token_bracket_open(lexer::position) override;
	virtual void token_bracket_close(lexer::position) override;
	virtual void token_brace_empty(lexer::position) override;
	virtual void token_brace_open(lexer::position) override;
	virtual void token_brace_close(lexer::position) override;
	virtual void token_comma(lexer::position) override;
	virtual void token_colon(lexer::position) override;
	virtual void token_semicolon(lexer::position) override;
	virtual void token_plus(lexer::position) override;
	virtual void token_hyphen(lexer::position) override;
	virtual void token_star(lexer::position) override;
	virtual void token_slash(lexer::position) override;
	virtual void token_percent(lexer::position) override;
	virtual void token_equal(lexer::position) override;
	virtual void token_diamond(lexer::position) override;
	virtual void token_angle_left(lexer::position) override;
	virtual void token_angle_right(lexer::position) override;
	virtual void token_ampersand(lexer::position) override;
	virtual void token_pipe(lexer::position) override;
	virtual void token_caret(lexer::position) override;
	virtual void token_shift_left(lexer::position) override;
	virtual void token_shift_right(lexer::position) override;
	virtual void token_arrow_left(lexer::position) override;
	virtual void token_arrow_right(lexer::position) override;
	void flush();
protected:
	void accept(int val);
	int recall();
	void parse_prefix(int);
	void parse_infix(int);
	void commit_op();
	bool close_group(int);
private:
	output &out;
	error &err;
};

#endif //PARSER_H

