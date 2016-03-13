// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef TOKEN_H
#define TOKEN_H

#include "location.h"
#include "process.h"
#include <string>

namespace token {
	struct number { std::string text; location loc; };
	struct identifier { std::string text; location loc; };
	struct string { std::string text; location loc; };
	struct symbol { std::string text; location loc; };
	struct delimiter { std::string text; location loc; };

	using delegate = output<
		number, identifier, string, symbol, delimiter
	>;
};

#endif //TOKEN_H
