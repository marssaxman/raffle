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
#include <sstream>

class lexer {
public:
	lexer(token::delegate &o, errors &e): out(o), err(e) {}
	void scan(char);
	void flush();
private:
	void accept(char);
	void reject(char);
	void clear();
	void emit(token::type);
	int state = 0;
	position tk_begin;
	position tk_end;
	std::stringstream buf;
	token::delegate &out;
	errors &err;
};

#endif //LEXER_H

