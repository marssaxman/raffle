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

static void run(std::istream &src)
{
	errors e;
	ast o;
	parser p(o, e);
	lexer l(p, e);
	l.read_file(std::cin);
	p.close();
}

int main(int argc, const char *argv[]) {
	if (argc <= 1) {
		run(std::cin);
	} else for (int i = 1; i < argc; ++i) {
		std::ifstream file(argv[i]);
		run(file);
	}
	return EXIT_SUCCESS;
}

