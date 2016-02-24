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

struct output: public ast::builder {
	std::stack<std::string> nodes;
	virtual void build_atom(location l, ast::atom::tag id) override {
		switch (id) {
			case ast::atom::wildcard: nodes.push("_"); break;
			case ast::atom::null: nodes.push("?"); break;
		}
	}
	virtual void build_leaf(
			location l, ast::leaf::tag id, std::string t) override {
		nodes.push(t);
	}
	virtual void build_branch(location l, ast::branch::tag t) override {
		std::string right = nodes.top();
		nodes.pop();
		std::string left = nodes.top();
		nodes.pop();
		std::string opname;
		switch (t) {
			case ast::branch::add: opname = "+"; break;
			case ast::branch::sub: opname = "-"; break;
			case ast::branch::mul: opname = "*"; break;
			case ast::branch::div: opname = "/"; break;
			case ast::branch::rem: opname = "%"; break;
			case ast::branch::shl: opname = "<<"; break;
			case ast::branch::shr: opname = ">>"; break;
			case ast::branch::conjoin: opname = "&"; break;
			case ast::branch::disjoin: opname = "|"; break;
			case ast::branch::exclude: opname = "^"; break;
			case ast::branch::eq: opname = "="; break;
			case ast::branch::lt: opname = "<"; break;
			case ast::branch::gt: opname = ">"; break;
			case ast::branch::neq: opname = "!="; break;
			case ast::branch::nlt: opname = "!<"; break;
			case ast::branch::ngt: opname = "!>"; break;
			case ast::branch::apply: opname = "apply"; break;
			case ast::branch::invoke: opname = "call"; break;
			case ast::branch::slice: opname = "slice"; break;
			case ast::branch::extend: opname = "extend"; break;
			case ast::branch::pipeline: opname = "."; break;
			case ast::branch::assign: opname = "<-"; break;
			case ast::branch::capture: opname = "->"; break;
			case ast::branch::declare: opname = ":"; break;
			case ast::branch::define: opname = ":="; break;
			case ast::branch::typealias: opname = "::="; break;
			case ast::branch::range: opname = ".."; break;
			case ast::branch::sequence: opname = ";"; break;
			case ast::branch::pair: opname = ","; break;
		}
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

