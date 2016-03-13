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
#include "process.h"
#include <string>
#include <sstream>

// lexical grammar:
// 	comment: # [^\n]*
// 	number: [0-9]+
// 	string: \" [^\"]* \"
// 	identifier: [A-Za-z_] [A-Za-z0-9_]*
//  symbol: [\+\-\*\/\%\<\>\|\&\^\!\=\.\:]+
//  delimiter: [\(\)\[\]\{\}\;\,]
// 	space: [ \t\v\f]+

class lexer: public output<char> {
public:
	lexer(token::delegate &o, errors &e): out(o), err(e) {}
	virtual void flush() override;
	virtual void operator<<(char) override;
private:
	void accept(char);
	void reject(char);
	void clear();
	template <typename T>
	void emit() {
		out << T{buf.str(), location(tk_begin, tk_end)};
		clear();
	}
	enum {
		start = 0,
		comment,
		number,
		string,
		identifier,
		symbol,
		space,
		eof
	} state = start;
	position tk_begin;
	position tk_end;
	std::stringstream buf;
	token::delegate &out;
	errors &err;
};

#endif //LEXER_H

