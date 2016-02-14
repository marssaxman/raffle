// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <iostream>
#include "position.h"

// lexical grammar:
// 	file: (line '\n')*
// 	line: token* comment?
// 	comment: '#' [^\n]*
// 	token: number | symbol | string | space | blank | delimiter | operator
// 	number: [0-9]+
// 	symbol: ([A-Za-z] [_A-Za-z0-9]*
// 	string: ('\"' [^\"]* '\"') | ('\'' [^\']* '\'')
// 	space: [\s\t]+
//	blank: '_' | '()' | '[]' | '{}'
// 	delimiter: '(' | '[' | '{' | '}' | ']' | ')'
// 	operator: arithmetic | relation | structure
//	arithmetic: '+' | '-' | '*' | '/' | '%' | '<<' | '>>'
//  logic/set: '|' | '&' | '^' | '!'
//	relation: '=' | '<' | '>' | '!=' | '!<' | '!>'
//	structure: ',' | ':' | ';' | '<-' | '->' | '.' | '..'

class lexer {
public:
	struct output {
		virtual void token_number(position, std::string) = 0;
		virtual void token_symbol(position, std::string) = 0;
		virtual void token_string(position, std::string) = 0;
		virtual void token_blank(position) = 0;
		virtual void token_paren_empty(position) = 0;
		virtual void token_paren_open(position) = 0;
		virtual void token_paren_close(position) = 0;
		virtual void token_bracket_empty(position) = 0;
		virtual void token_bracket_open(position) = 0;
		virtual void token_bracket_close(position) = 0;
		virtual void token_brace_empty(position) = 0;
		virtual void token_brace_open(position) = 0;
		virtual void token_brace_close(position) = 0;
		virtual void token_comma(position) = 0;
		virtual void token_semicolon(position) = 0;
		virtual void token_colon(position) = 0;
		virtual void token_dot(position) = 0;
		virtual void token_dot_dot(position) = 0;
		virtual void token_plus(position) = 0;
		virtual void token_hyphen(position) = 0;
		virtual void token_star(position) = 0;
		virtual void token_slash(position) = 0;
		virtual void token_percent(position) = 0;
		virtual void token_ampersand(position) = 0;
		virtual void token_pipe(position) = 0;
		virtual void token_caret(position) = 0;
		virtual void token_bang(position) = 0;
		virtual void token_equal(position) = 0;
		virtual void token_lesser(position) = 0;
		virtual void token_greater(position) = 0;
		virtual void token_bang_equal(position) = 0;
		virtual void token_bang_lesser(position) = 0;
		virtual void token_bang_greater(position) = 0;
		virtual void token_shift_left(position) = 0;
		virtual void token_shift_right(position) = 0;
		virtual void token_arrow_left(position) = 0;
		virtual void token_arrow_right(position) = 0;
	};
	struct error {
		virtual void lexer_unknown(position, char) = 0;
		virtual void lexer_nonterminated(position) = 0;
	};
	lexer(output &o, error &e): out(o), err(e) {}
	void read_line(const std::string&);
	void read_file(std::istream &in);
private:
	position pos;
	output &out;
	error &err;
};

#endif //LEXER_H

