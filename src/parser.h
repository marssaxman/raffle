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
		location loc;
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
		virtual void parser_unexpected(location) = 0;
		virtual void parser_missing_operand(location) = 0;
		virtual void parser_mismatched_paren(location) = 0;
		virtual void parser_mismatched_bracket(location) = 0;
		virtual void parser_mismatched_brace(location) = 0;
		virtual void parser_unimplemented(location) = 0;
	};
	parser(output &o, error &e): out(o), err(e) {}
	virtual void token_number(location, std::string) override;
	virtual void token_symbol(location, std::string) override;
	virtual void token_string(location, std::string) override;
	virtual void token_blank(location) override;
	virtual void token_paren_empty(location) override;
	virtual void token_paren_open(location) override;
	virtual void token_paren_close(location) override;
	virtual void token_bracket_empty(location) override;
	virtual void token_bracket_open(location) override;
	virtual void token_bracket_close(location) override;
	virtual void token_brace_empty(location) override;
	virtual void token_brace_open(location) override;
	virtual void token_brace_close(location) override;
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
	virtual void token_lesser(location) override;
	virtual void token_greater(location) override;
	virtual void token_bang_equal(location) override;
	virtual void token_bang_lesser(location) override;
	virtual void token_bang_greater(location) override;
	virtual void token_bang(location) override;
	virtual void token_ampersand(location) override;
	virtual void token_pipe(location) override;
	virtual void token_caret(location) override;
	virtual void token_shift_left(location) override;
	virtual void token_shift_right(location) override;
	virtual void token_arrow_left(location) override;
	virtual void token_arrow_right(location) override;
	void flush();
protected:
	void accept(int val);
	int recall();
	void parse_prefix(int, location);
	void parse_infix(int, location);
	void commit_op();
	bool close_group(int);
private:
	output &out;
	error &err;
};

#endif //PARSER_H

