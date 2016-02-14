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
	struct token {
		int id;
		position pos;
	};
	std::stack<int> values;
	std::stack<token> states;
	// some tokens apply in either infix or prefix context
	bool prefix = true;
public:
	struct output {
		virtual int rule_number(std::string) = 0;
		virtual int rule_symbol(std::string) = 0;
		virtual int rule_string(std::string) = 0;
		virtual int rule_blank() = 0;
		virtual int rule_paren_empty() = 0;
		virtual int rule_bracket_empty() = 0;
		virtual int rule_brace_empty() = 0;
		virtual int rule_sequence(int, int) = 0;
		virtual int rule_capture(int, int) = 0;
		virtual int rule_define(int, int) = 0;
		virtual int rule_list(int, int) = 0;
		virtual int rule_caption(int, int) = 0;
		virtual int rule_equal(int, int) = 0;
		virtual int rule_lesser(int, int) = 0;
		virtual int rule_greater(int, int) = 0;
		virtual int rule_not_equal(int, int) = 0;
		virtual int rule_not_lesser(int, int) = 0;
		virtual int rule_not_greater(int, int) = 0;
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
		virtual int rule_paren_group(int) = 0;
		virtual int rule_bracket_group(int) = 0;
		virtual int rule_brace_group(int) = 0;
		virtual int rule_subscript(int, int) = 0;
		virtual int rule_lookup(int, int) = 0;
	};
	struct error {
		virtual void parser_unexpected(position) = 0;
		virtual void parser_missing_operand(position) = 0;
		virtual void parser_mismatched_paren(position) = 0;
		virtual void parser_mismatched_bracket(position) = 0;
		virtual void parser_mismatched_brace(position) = 0;
		virtual void parser_unimplemented(position) = 0;
	};
	parser(output &o, error &e): out(o), err(e) {}
	virtual void token_number(position p, std::string text) override;
	virtual void token_symbol(position p, std::string text) override;
	virtual void token_string(position p, std::string text) override;
	virtual void token_blank(position p) override;
	virtual void token_paren_empty(position) override;
	virtual void token_paren_open(position) override;
	virtual void token_paren_close(position) override;
	virtual void token_bracket_empty(position) override;
	virtual void token_bracket_open(position) override;
	virtual void token_bracket_close(position) override;
	virtual void token_brace_empty(position) override;
	virtual void token_brace_open(position) override;
	virtual void token_brace_close(position) override;
	virtual void token_comma(position) override;
	virtual void token_colon(position) override;
	virtual void token_semicolon(position) override;
	virtual void token_dot(position) override;
	virtual void token_dot_dot(position) override;
	virtual void token_plus(position) override;
	virtual void token_hyphen(position) override;
	virtual void token_star(position) override;
	virtual void token_slash(position) override;
	virtual void token_percent(position) override;
	virtual void token_equal(position) override;
	virtual void token_lesser(position) override;
	virtual void token_greater(position) override;
	virtual void token_bang_equal(position) override;
	virtual void token_bang_lesser(position) override;
	virtual void token_bang_greater(position) override;
	virtual void token_bang(position) override;
	virtual void token_ampersand(position) override;
	virtual void token_pipe(position) override;
	virtual void token_caret(position) override;
	virtual void token_shift_left(position) override;
	virtual void token_shift_right(position) override;
	virtual void token_arrow_left(position) override;
	virtual void token_arrow_right(position) override;
	void flush();
protected:
	void accept(int val);
	int recall();
	void parse_prefix(int, position);
	void parse_infix(int, position);
	void commit_op();
	bool close_group(int);
private:
	output &out;
	error &err;
};

#endif //PARSER_H

