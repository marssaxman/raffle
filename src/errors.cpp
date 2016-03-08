// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "errors.h"
#include <iostream>

void errors::print_loc(location l) {
	position p = l.begin;
	std::cerr << p.row() << ":" << p.col();
}

void errors::report(location l, std::string message) {
	print_loc(l);
	std::cerr << ": " << message << std::endl;
}

void errors::report(location l, std::string message, location prev) {
	print_loc(l);
	std::cerr << ": " << message << " (see ";
	print_loc(prev);
	std::cerr << ")" << std::endl;
}

