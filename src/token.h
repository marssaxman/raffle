// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef TOKEN_H
#define TOKEN_H

#include "location.h"
#include <string>

struct token {
	token(std::string t, location l): text(t), loc(l) {}
	std::string text;
	location loc;
	struct delegate {
		virtual void token_eof(token) = 0;
		virtual void token_number(token) = 0;
		virtual void token_identifier(token) = 0;
		virtual void token_string(token) = 0;
		virtual void token_open(token) = 0;
		virtual void token_close(token) = 0;
		virtual void token_symbol(token) = 0;
	};
};

#endif //TOKEN_H
