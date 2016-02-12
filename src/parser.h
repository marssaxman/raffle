// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stack>

class parser: public lexer::output {
	struct block {
		enum id {
			root = 0,
			paren,
			bracket,
			brace
		} type;
		unsigned count;
	};
	std::stack<block> state;
public:
	struct output {
		virtual void parse_number(std::string) = 0;
		virtual void parse_symbol(std::string) = 0;
		virtual void parse_string(std::string) = 0;
		virtual void parse_paren_group(unsigned count) = 0;
		virtual void parse_bracket_group(unsigned count) = 0;
		virtual void parse_brace_group(unsigned count) = 0;
	};
	struct error {
		virtual void parse_expect_paren(lexer::position) = 0;
		virtual void parse_expect_bracket(lexer::position) = 0;
		virtual void parse_expect_brace(lexer::position) = 0;
		virtual void parse_unexpected_close(lexer::position) = 0;
	};
	parser(output &o, error &e): out(o), err(e) { state.push({}); }
	virtual void token_number(lexer::position p, std::string text) override;
	virtual void token_symbol(lexer::position p, std::string text) override;
	virtual void token_string(lexer::position p, std::string text) override;
	virtual void token_paren_open(lexer::position) override;
	virtual void token_paren_close(lexer::position) override;
	virtual void token_bracket_open(lexer::position) override;
	virtual void token_bracket_close(lexer::position) override;
	virtual void token_brace_open(lexer::position) override;
	virtual void token_brace_close(lexer::position) override;
	virtual void token_comma(lexer::position) override;
	virtual void token_semicolon(lexer::position) override;
protected:
	bool close_block(block::id, lexer::position);
	void unexpected_close(lexer::position);
private:
	output &out;
	error &err;
};

#endif //PARSER_H

