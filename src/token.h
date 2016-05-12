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
enum type {
	eof,
	number,
	identifier,
	string,
	symbol,
	delimiter
};
struct delegate {
	virtual void parse(enum type, std::string, location) = 0;
};
}

#endif //TOKEN_H
