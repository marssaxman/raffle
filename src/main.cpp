// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "errors.h"
#include "printer.h"

struct receiver: public ast::delegate {
	virtual void ast_process(ast::ptr &&n) override {
		printer p(std::cout);
		n->accept(p);
		std::cout << ";" << std::endl;
	}
	virtual void ast_done() override {
		std::cout << "$> ";
	}
};

int main(int argc, const char *argv[]) {
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

