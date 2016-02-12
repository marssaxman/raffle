// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"

void parser::token_number(lexer::position p, std::string text)
{
	out.parse_number(text);
	++state.top().count;
}

void parser::token_symbol(lexer::position p, std::string text)
{
	out.parse_symbol(text);
	++state.top().count;
}

void parser::token_string(lexer::position p, std::string text)
{
	out.parse_string(text);
	++state.top().count;
}

void parser::token_paren_open(lexer::position p)
{
	state.push({block::paren});
}

void parser::token_paren_close(lexer::position p)
{
	if (close_block(block::paren, p)) {
		out.parse_paren_group(state.top().count);
		state.pop();
	}
}

void parser::token_bracket_open(lexer::position p)
{
	state.push({block::bracket});
}

void parser::token_bracket_close(lexer::position p)
{
	if (close_block(block::brace, p)) {
		out.parse_bracket_group(state.top().count);
		state.pop();
	}
}

void parser::token_brace_open(lexer::position p)
{
	state.push({block::brace});
}

void parser::token_brace_close(lexer::position p)
{
	if (close_block(block::brace, p)) {
		out.parse_brace_group(state.top().count);
		state.pop();
	}
}

void parser::token_comma(lexer::position p)
{
	std::cout << "comma" << std::endl;
}

void parser::token_semicolon(lexer::position p)
{
	std::cout << "semicolon" << std::endl;
}

bool parser::close_block(block::id type, lexer::position p)
{
	return state.top().type == type || (unexpected_close(p), 0);
}

void parser::unexpected_close(lexer::position p)
{
	switch (state.top().type) {
		case block::paren: err.parse_expect_paren(p); break;
		case block::bracket: err.parse_expect_bracket(p); break;
		case block::brace: err.parse_expect_brace(p); break;
		default: err.parse_unexpected_close(p); break;
	}
}


