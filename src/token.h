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
struct delegate {
	virtual void token_eof(location) = 0;
	virtual void token_number(std::string, location) = 0;
	virtual void token_identifier(std::string, location) = 0;
	virtual void token_string(std::string, location) = 0;
	virtual void token_underscore(location) = 0;
	virtual void token_open(std::string, location) = 0;
	virtual void token_close(std::string, location) = 0;
	virtual void token_symbol(std::string, location) = 0;
};

} // namespace token

#endif //TOKEN_H
