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
#include "error_handler.h"

struct ast
{
	std::string value;
	std::deque<ast> tree;
};

static void print_ast(ast node, std::string indent)
{
	std::cout << indent << node.value << std::endl;
	while (!node.tree.empty()) {
		ast sub = node.tree.front();
		node.tree.pop_front();
		print_ast(sub, indent + "  ");
	}
}

struct output_handler: public parser::output {
	std::stack<ast> state;
	virtual void parse_number(std::string t) override;
	virtual void parse_symbol(std::string t) override;
	virtual void parse_string(std::string t) override;
	virtual void parse_paren_group(unsigned count) override;
	virtual void parse_bracket_group(unsigned count) override;
	virtual void parse_brace_group(unsigned count) override;
	void printout()
	{
		std::stack<ast> backup;
		unsigned i = 0;
		while (!state.empty()) {
			ast n = state.top();
			backup.push(n);
			state.pop();
			print_ast(n, std::to_string(i++) + ": ");
		}
		while (!backup.empty()) {
			state.push(backup.top());
			backup.pop();
		}
		std::cout << std::endl;
	}
private:
	void fill_group(ast&, unsigned count);
};

void output_handler::parse_number(std::string t)
{
	state.push({t});
	printout();
}

void output_handler::parse_symbol(std::string t)
{
	state.push({t});
	printout();
}

void output_handler::parse_string(std::string t)
{
	state.push({t});
	printout();
}

void output_handler::parse_paren_group(unsigned count)
{
	ast group = {"()"};
	fill_group(group, count);
	printout();
}

void output_handler::parse_bracket_group(unsigned count)
{
	ast group = {"[]"};
	fill_group(group, count);
	printout();
}

void output_handler::parse_brace_group(unsigned count)
{
	ast group = {"{}"};
	fill_group(group, count);
	printout();
}

void output_handler::fill_group(ast &group, unsigned count)
{
	while (count-- > 0) {
		group.tree.push_front(state.top());
		state.pop();
	}
	state.push(group);
}

int main(int argc, const char *argv[]) {
	error_handler e;
	output_handler o;
	parser p(o, e);
	if (argc <= 1) {
		lexer l(p, e);
		l.read_file(std::cin);
	} else for (int i = 1; i < argc; ++i) {
		lexer l(p, e);
		std::ifstream file(argv[i]);
		l.read_file(file);
	}
	return EXIT_SUCCESS;
}

