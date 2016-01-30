// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef SCANNER_H
#define SCANNER_H

#include <climits>
#include "pipeline.h"

struct chartype
{
	enum class id
	{
		error,
		space,
		punct,
		alpha,
		digit,
		newln,
		other
	} type;
	char value;
};

struct scanner: public filter<char, chartype>
{
	scanner();
	chartype apply(char in) override;
private:
	chartype::id table[CHAR_MAX];
};

#endif //SCANNER_H

