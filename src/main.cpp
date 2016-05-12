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

using std::string;

struct readout: public lts::builder {
	std::stack<string> nodes;
	virtual void lts_null(location l) override {
		nodes.push("null");
	}
	virtual void lts_number(string t, location l) override {
		nodes.push(t);
	}
	virtual void lts_string(string t, location l) override {
		nodes.push(t);
	}
	virtual void lts_symbol(string t, location l) override {
		nodes.push("`" + t + "`");
	}
	virtual void lts_apply_top(location l) override {
		string arg = pop();
		string func = pop();
		nodes.push(func + " " + arg);
	}
	virtual void lts_apply_next(location l) override {
		string func = pop();
		string arg = pop();
		nodes.push(func + " " + arg);
	}
	virtual void lts_lambda(location l) override {
		string exp = pop();
		string param = pop();
		nodes.push("lambda " + param + " -> " + exp);
	}
	virtual void lts_bind(location l) override {
		string val = pop();
		string key = pop();
		nodes.push("bind " + key + " -> " + val);
	}
	virtual void lts_eval(location l) override {
		string exp = pop();
		string ctx = pop();
		nodes.push("eval " + ctx + " -> " + exp);
	}
	string pop() {
		string v = nodes.top();
		nodes.pop();
		return (v.find_first_of(' ') != string::npos) ? ("(" + v + ")"): v;
	}
	void print() {
		if (nodes.empty()) {
			std::cout << "expression stack is empty" << std::endl;
			return;
		}
		string top = nodes.top();
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
	readout o;
	binder r(o, e);
	parser p(r, e);
	lexer l(p, e);
	char c;
	while (i.get(c)) {
		l.scan(c);
	}
	l.scan(0);
	o.print();
	return 0;
}

int main(int argc, const char *argv[]) {
	if (argc <= 1 && isatty(fileno(stdin))) {
		std::cout << "$> ";
		for (std::string line; std::getline(std::cin, line);) {
			std::stringstream in(line);
			run(in);
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

