// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef ERRORS_H
#define ERRORS_H

#include "lexer.h"
#include "parser.h"
#include "resolver.h"

struct errors:
		public lexer::error,
		public parser::error,
		public resolver::error {
	virtual void lexer_unknown(location, char) override;
	virtual void lexer_nonterminated(location) override;
	virtual void parser_unexpected(location) override;
	virtual void parser_missing_left_operand(location) override;
	virtual void parser_missing_right_operand(location) override;
	virtual void parser_expected(location, std::string, location) override;
	virtual void resolver_undefined(location) override;
	virtual void resolver_redefined(location, location) override;
	virtual void resolver_unexpected_definition(location) override;
	virtual void resolver_unexpected_target(location) override;
	virtual void resolver_unexpected_constraint(location) override;
	virtual void resolver_unexpected_wildcard(location) override;
private:
	void print_loc(location);
	void report(location, std::string message);
	void report(location, std::string message, location);
};

#endif //ERRORS_H

