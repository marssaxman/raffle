// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef LTS_H
#define LTS_H

#include "location.h"
#include <string>

namespace lts {
struct builder {
	virtual void lts_null(location) = 0;
	virtual void lts_number(std::string, location) = 0;
	virtual void lts_string(std::string, location) = 0;
	virtual void lts_symbol(std::string, location) = 0;
	virtual void lts_apply_top(location) = 0; // pop arg / apply it to next
	virtual void lts_apply_next(location) = 0; // pop func / apply next to it
	virtual void lts_lambda(location) = 0;
	virtual void lts_bind(location) = 0;
	virtual void lts_eval(location) = 0;
};
} // namespace lts

#endif //LTS_H
