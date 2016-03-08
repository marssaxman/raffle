// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef ERRORS_H
#define ERRORS_H

#include "location.h"
#include <string>

struct errors {
	void report(location where, std::string message);
	void report(location where, std::string message, location previous);
private:
	void print_loc(location);
};

#endif //ERRORS_H

