// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include <iostream>
#include <fstream>
#include <deque>
#include <stack>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "errors.h"

int main(int argc, const char *argv[]) {
	errors e;
	ast o;
	parser p(o, e);
	lexer l(p, e);
	for (std::string line; std::getline(std::cin, line);) {
		l.read_line(line);
		p.flush();
		std::cout << std::endl;
	}
	return EXIT_SUCCESS;
}

