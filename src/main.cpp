// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include <iostream>
#include <fstream>
#include "lexer.h"

struct output_handler: public lexer::output {
	virtual void indent(lexer::position p, unsigned level) {}
	virtual void spacer(lexer::position p) override {}
	virtual void number(lexer::position p, std::string text) override {}
	virtual void symbol(lexer::position p, std::string text) override {}
	virtual void string(lexer::position p, std::string text) override {}
	virtual void rubric(lexer::position p, std::string text) override {}
	virtual void opener(lexer::position p, char c) override {}
	virtual void closer(lexer::position p, char c) override {}
	virtual void newline(lexer::position p) override {}
};

struct error_handler: public lexer::error {
	virtual void unknown(lexer::position p, char c) override {}
	virtual void nonterminated(lexer::position p) override {}
};

int main(int argc, const char *argv[]) {
	output_handler o;
	error_handler e;
	if (argc <= 1) {
		lexer l(o, e);
		l.read_file(std::cin);
	} else for (int i = 1; i < argc; ++i) {
		lexer l(o, e);
		std::ifstream file(argv[i]);
		l.read_file(file);
	}
	return EXIT_SUCCESS;
}

