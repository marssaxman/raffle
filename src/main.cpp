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

#include <unistd.h>

struct output: public ast::builder {
	virtual void build_blank(location l) override {
		std::cout << "_ ";
	}
	virtual void build_leaf(
			ast::leaf::tag id, std::string t, location l) override {
		if (id == ast::leaf::string) {
			std::cout << "\"" << t << "\"";
		} else {
			std::cout << t;
		}
		std::cout << " ";
	}
	virtual void build_branch(ast::branch::tag t, location l) override {
		switch (t) {
			case ast::branch::add: std::cout << "+ "; break;
			case ast::branch::sub: std::cout << "- "; break;
			case ast::branch::mul: std::cout << "* "; break;
			case ast::branch::div: std::cout << "/ "; break;
			case ast::branch::rem: std::cout << "% "; break;
			case ast::branch::shl: std::cout << "<< "; break;
			case ast::branch::shr: std::cout << ">> "; break;
			case ast::branch::conjoin: std::cout << "& "; break;
			case ast::branch::disjoin: std::cout << "| "; break;
			case ast::branch::exclude: std::cout << "^ "; break;
			case ast::branch::eq: std::cout << "= "; break;
			case ast::branch::lt: std::cout << "< "; break;
			case ast::branch::gt: std::cout << "> "; break;
			case ast::branch::neq: std::cout << "!= "; break;
			case ast::branch::nlt: std::cout << "!< "; break;
			case ast::branch::ngt: std::cout << "!> "; break;
			case ast::branch::apply: break;
			case ast::branch::pipeline: std::cout << "."; break;
			case ast::branch::assign: std::cout << "<- "; break;
			case ast::branch::capture: std::cout << "-> "; break;
			case ast::branch::declare: std::cout << ":= "; break;
			case ast::branch::define: std::cout << ": "; break;
			case ast::branch::typealias: std::cout << "::= "; break;
			case ast::branch::range: std::cout << ".."; break;
			case ast::branch::sequence: std::cout << "; "; break;
			case ast::branch::tuple: std::cout << ", "; break;
		}
	}
};

static int run(std::istream &i) {
	errors e;
	output o;
	parser p(o, e);
	lexer l(p, e);
	l.read_file(i);
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

