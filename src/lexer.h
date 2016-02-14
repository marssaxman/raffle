// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <iostream>
#include "location.h"

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
		enum class direction {
			left,
			right
		};
		virtual void token_number(location, std::string) = 0;
		virtual void token_symbol(location, std::string) = 0;
		virtual void token_string(location, std::string) = 0;
		virtual void token_underscore(location) = 0;
		virtual void token_paren_pair(location) = 0;
		virtual void token_paren(location, direction) = 0;
		virtual void token_bracket_pair(location) = 0;
		virtual void token_bracket(location, direction) = 0;
		virtual void token_brace_pair(location) = 0;
		virtual void token_brace(location, direction) = 0;
		virtual void token_comma(location) = 0;
		virtual void token_semicolon(location) = 0;
		virtual void token_colon(location) = 0;
		virtual void token_dot(location) = 0;
		virtual void token_dot_dot(location) = 0;
		virtual void token_plus(location) = 0;
		virtual void token_hyphen(location) = 0;
		virtual void token_star(location) = 0;
		virtual void token_slash(location) = 0;
		virtual void token_percent(location) = 0;
		virtual void token_ampersand(location) = 0;
		virtual void token_pipe(location) = 0;
		virtual void token_caret(location) = 0;
		virtual void token_bang(location) = 0;
		virtual void token_equal(location) = 0;
		virtual void token_angle(location, direction) = 0;
		virtual void token_bang_equal(location) = 0;
		virtual void token_bang_angle(location, direction) = 0;
		virtual void token_guillemet(location, direction) = 0;
		virtual void token_arrow(location, direction) = 0;
	};
	typedef output::direction direction;
	struct error {
		virtual void lexer_unknown(location, char) = 0;
		virtual void lexer_nonterminated(location) = 0;
	};
	lexer(output &o, error &e): out(o), err(e) {}
	void read_line(const std::string&);
	void read_file(std::istream &in);
private:
	void next(std::string::const_iterator &i, std::string::const_iterator end);
	std::string::const_iterator &adv(std::string::const_iterator &i);
	position pos;
	location loc;
	output &out;
	error &err;
};

#endif //LEXER_H

