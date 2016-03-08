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
#include "binder.h"
#include "errors.h"

struct output: public lts::delegate {
	std::stack<std::string> nodes;
	virtual void lts_atom(location l, lts::atom id) override {
		std::string t;
		switch (id) {
			case lts::atom::param: t = "arg"; break;
			case lts::atom::env: t = "env"; break;
			case lts::atom::null: t = "null"; break;
			case lts::atom::echo: t = "echo"; break;
		}
		nodes.push(t);
	}
	virtual void lts_leaf(location l, lts::leaf id, std::string t) override {
		nodes.push((id == lts::leaf::symbol)? "`" + t + "`": t);
	}
	virtual void lts_branch(location l, lts::branch id) override {
		std::string right = nodes.top();
		nodes.pop();
		std::string left = nodes.top();
		nodes.pop();
		if (left.find_first_of(' ') != std::string::npos) {
			left = "(" + left + ")";
		}
		if (right.find_first_of(' ') != std::string::npos) {
			right = "(" + right + ")";
		}
		std::string t;
		switch (id) {
			case lts::branch::apply: nodes.push(left + " " + right); return;
			case lts::branch::lambda: t = "\xC2\xA3"; break;
			case lts::branch::pair: nodes.push(left + ", " + right); return;
			case lts::branch::match: t = "\xC2\xBB"; break;
			case lts::branch::join: t = "\xC2\xA6"; break;
		}
		nodes.push(left + " " + t + " " + right);
	}
	virtual void lts_swap() override {
		std::string right = nodes.top();
		nodes.pop();
		std::string left = nodes.top();
		nodes.pop();
		nodes.push(right);
		nodes.push(left);
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
	binder r(o, e);
	parser p(r, e);
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
			binder r(o, e);
			parser p(r, e);
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

