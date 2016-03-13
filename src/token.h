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
	struct base {
		base(std::string t, location l): text(t), loc(l) {}
		std::string text;
		location loc;
	};
	struct number: base { using base::base; };
	struct identifier: base { using base::base; };
	struct string: base { using base::base; };
	struct symbol: base { using base::base; };
	struct delimiter: base { using base::base; };

	using delegate = output<
		number, identifier, string, symbol, delimiter
	>;
};

#endif //TOKEN_H
