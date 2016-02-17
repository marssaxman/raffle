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
#include "printast.h"

#include <unistd.h>

struct receiver: public ast::delegate {
	virtual void ast_process(ast::ptr &&n) override {
		printast printer(std::cout);
		n->accept(printer);
		std::cout << ";" << std::endl;
	}
	virtual void ast_done() override {
		std::cout << "$> ";
	}
};

static int run(std::istream &i) {
	errors e;
	receiver o;
	parser p(o, e);
	lexer l(p, e);
	l.read_file(i);
}

int main(int argc, const char *argv[]) {
	if (argc <= 1 && isatty(fileno(stdin))) {
		std::cout << "$> ";
		for (std::string line; std::getline(std::cin, line);) {
			errors e;
			receiver o;
			parser p(o, e);
			lexer l(p, e);
			l.read_line(line);
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

