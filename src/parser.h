// Copyright (C) 2016 Mars Saxman. All rights reserved.// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"
#include <stack>
#include <functional>

struct parser: public token::delegate {
	struct error {
		virtual void parser_unexpected(location) = 0;
		virtual void parser_missing_left_operand(location) = 0;
		virtual void parser_missing_right_operand(location) = 0;
		virtual void parser_expected(location, std::string, location) = 0;
	};
	parser(ast::ostream &o, error &e): out(o), err(e) {}
	virtual void token_eof(location) override;
	virtual void token_number(location, std::string) override;
	virtual void token_identifier(location, std::string) override;
	virtual void token_string(location, std::string) override;
	virtual void token_underscore(location) override;
	virtual void token_open(location, token::delim) override;
	virtual void token_close(location, token::delim) override;
	virtual void token_symbol(location, std::string) override;
private:
	// the classic shunting-yard algorithm
	enum class precedence {
		structure, //L
		binding, //R
		relation, //L
		additive, //L
		multiplicative, //L
		prefix, //R
		primary //L
	};
	// Current state of the expression being parsed
	struct state {
		void close(location, ast::ostream&, error&);
		void term(ast::node*);
		void symbol(location, std::string, error&);
	private:
		// infix operators in flight
		struct oprec {
			ast::binary::opcode id;
			precedence prec;
			location loc;
		};
		std::stack<oprec> ops;
		// most recent term or operator value
		ast::ptr exp;
		// other values awaiting operators
		std::stack<ast::ptr> vals;
		ast::ptr pop();
		ast::ptr cur();
		void commit();
		bool expecting_term();
		void prefix(oprec, error&);
		void infix(oprec, error&);
		void emit(ast::node*);
		void prep(precedence);
	} context;
	// Outer context for a subexpression
	struct group {
		group(location l, token::delim t, state &&s):
				loc(l), type(t), context(std::move(s)) {}
		// location of the left-bracket used to enter this group
		location loc;
		// type of bracket we must match to close the group
		token::delim type;
		// the state as it existed before we entered the subexpression
		state context;
	};
	// Grouping stack outside the current expression context
	std::stack<group> outer;
	ast::ostream &out;
	error &err;
};

#endif //PARSER_H

