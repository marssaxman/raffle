// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef TOKEN_H
#define TOKEN_H

#include "location.h"
#include <string>

namespace token {
enum class delim {
	paren,
	bracket,
	brace,
};
struct delegate {
	virtual void token_eof(location) = 0;
	virtual void token_number(location, std::string) = 0;
	virtual void token_identifier(location, std::string) = 0;
	virtual void token_string(location, std::string) = 0;
	virtual void token_underscore(location) = 0;
	virtual void token_open(location, delim) = 0;
	virtual void token_close(location, delim) = 0;
	virtual void token_symbol(location, std::string) = 0;
};

} // namespace token

#endif //TOKEN_H
