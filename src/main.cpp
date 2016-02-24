// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stack>

#include "lexer.h"
#include "parser.h"
#include "errors.h"

struct output: public ast::delegate {
	std::stack<std::string> nodes;
	virtual void ast_atom(location l, ast::atom::tag id) override {
		switch (id) {
			case ast::atom::wildcard: nodes.push("_"); break;
			case ast::atom::null: nodes.push("?"); break;
		}
	}
	virtual void ast_leaf(
			location l, ast::leaf::tag id, std::string t) override {
		nodes.push(t);
	}
	virtual void ast_branch(
			location l, ast::branch::tag t, std::string opname) override {
		std::string right = nodes.top();
		nodes.pop();
		std::string left = nodes.top();
		nodes.pop();
		nodes.push("(" + opname + " " + left + " " + right + ")");
	}
	void print() {
		if (nodes.empty()) {
			std::cout << "expression stack is empty" << std::endl;
			return;
		}
		std::string top = nodes.top();
		nodes.pop();
		if (nodes.empty()) {
			std::cout << top << std::endl;
			return;
		}
		unsigned level = 0;
		do {
			std::cout << level++ << ": " << top << std::endl;
			top = nodes.top();
			nodes.pop();
		} while (!nodes.empty());
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

