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
#include "token.h"

// lexical grammar:
// 	file: (line '\n')*
// 	line: token* comment?
// 	comment: '#' [^\n]*
// 	token: number | symbol | string | space | blank | delimiter | operator
// 	number: [0-9]+
// 	symbol: [A-Za-z] [_A-Za-z0-9]*
// 	string: ('\"' [^\"]* '\"') | ('\'' [^\']* '\'')
// 	space: [\s\t]+
//	blank: '_'
// 	delimiter: '(' | '[' | '{' | '}' | ']' | ')'
// 	operator: arithmetic | relation | structure
//	arithmetic: '+' | '-' | '*' | '/' | '%' | '<<' | '>>'
//  logic/set: '|' | '&' | '^' | '!'
//	relation: '=' | '<' | '>' '!=' | '!<' | '!>'
//	structure: ',' | ':' | ';' | '<-' | '->' | '.' | '..'

class lexer {
	typedef token::direction direction;
public:
	struct error {
		virtual void lexer_unknown(location, char) = 0;
		virtual void lexer_nonterminated(location) = 0;
	};
	lexer(token::delegate &o, error &e): out(o), err(e) {}
	void read_line(const std::string&);
	void read_file(std::istream &in);
private:
	void next(std::string::const_iterator &i, std::string::const_iterator end);
	std::string::const_iterator &adv(std::string::const_iterator &i);
	position pos;
	location loc;
	token::delegate &out;
	error &err;
};

#endif //LEXER_H

