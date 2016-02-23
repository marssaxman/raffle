// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "errors.h"
#include "printer.h"

#include <unistd.h>

struct output: public ast::ostream {
	virtual ast::ostream &operator<<(ast::ptr &&n) override {
		result = std::move(n);
	}
	ast::ptr result;
	void print() {
		printer p(std::cout);
		if (result) result->accept(p);
		std::cout << std::endl;
	}
};

static int run(std::istream &i) {
	errors e;
	output o;
	parser p(o, e);
	lexer l(p, e);
	l.read_file(i);
	o.print();
}

int main(int argc, const char *argv[]) {
	if (argc <= 1 && isatty(fileno(stdin))) {
		std::cout << "$> ";
		for (std::string line; std::getline(std::cin, line);) {
			errors e;
			output o;
			parser p(o, e);
			lexer l(p, e);
			l.read_line(line);
			o.print();
			std::cout << std::endl << "$> ";
		}
		return EXIT_SUCCESS;
	}
	if (argc <= 1) {
		return run(std::cin);
	}
	for (int i = 1; i < argc; ++i) {
		std::ifstream file(argv[i]);
		int ret = run(file);
		if (ret) return ret;
	}
	return EXIT_SUCCESS;
}

