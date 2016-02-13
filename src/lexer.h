// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <iostream>

// lexical grammar:
// 	file: (line '\n')*
// 	line: token* comment?
// 	comment: '#' [^\n]*
// 	token: number | symbol | string | space | null | delimiter | operator
// 	number: [0-9]+
// 	symbol: [_A-Za-z] [_A-Za-z0-9]*
// 	string: ('\"' [^\"]* '\"') | ('\'' [^\']* '\'')
// 	space: [\s\t]+
//	null: '()' | '[]' | '{}'
// 	delimiter: '(' | '[' | '{' | '}' | ']' | ')'
// 	operator: arithmetic | relation | structure
//	arithmetic: '+' | '-' | '*' | '/' | '%' | '|' | '^' | '&' | '<<' | '>>'
//	relation: '=' | '<>' | '<' | '>'
//	structure: ',' | ':' | ';' | '<-' | '->'

class lexer {
public:
	struct position {
		unsigned row = 0;
		unsigned col = 0;
	};
	struct output {
		virtual void token_number(position, std::string) = 0;
		virtual void token_symbol(position, std::string) = 0;
		virtual void token_string(position, std::string) = 0;
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
		virtual void token_plus(position) = 0;
		virtual void token_hyphen(position) = 0;
		virtual void token_star(position) = 0;
		virtual void token_slash(position) = 0;
		virtual void token_percent(position) = 0;
		virtual void token_ampersand(position) = 0;
		virtual void token_pipe(position) = 0;
		virtual void token_caret(position) = 0;
		virtual void token_equal(position) = 0;
		virtual void token_diamond(position) = 0;
		virtual void token_angle_left(position) = 0;
		virtual void token_angle_right(position) = 0;
		virtual void token_shift_left(position) = 0;
		virtual void token_shift_right(position) = 0;
		virtual void token_arrow_left(position) = 0;
		virtual void token_arrow_right(position) = 0;
	};
	struct error {
		virtual void token_unknown(position, char) = 0;
		virtual void token_nonterminated(position) = 0;
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

