// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef LEXER_H
#define LEXER_H

#include "errors.h"
#include "location.h"
#include "token.h"
#include <string>
#include <iostream>

// lexical grammar:
// 	comment: # [^\n]*
// 	number: [0-9]+
// 	string: \" [^\"]* \"
// 	identifier: [A-Za-z_] [A-Za-z0-9_]*
//  symbol: [\+\-\*\/\%\<\>\|\&\^\!\=\.\:]+
//  delimiter: [\(\)\[\]\{\}\;\,]
// 	space: [ \t\v\f]+

class lexer {
public:
	lexer(token::delegate &o, errors &e): out(o), err(e) {}
	void read_line(const std::string&);
	void read_file(std::istream &in);
private:
	void read(const std::string&);
	void next();
	std::string::const_iterator tokenstart;
	std::string::const_iterator i;
	std::string::const_iterator end;
	position pos;
	token current() const;
	token::delegate &out;
	errors &err;
};

#endif //LEXER_H

