// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef ERRORS_H
#define ERRORS_H

#include "lexer.h"
#include "parser.h"

struct errors: public lexer::error, public parser::error {
	virtual void lexer_unknown(location, char) override;
	virtual void lexer_nonterminated(location) override;
	virtual void parser_unexpected(location) override;
	virtual void parser_mismatched_delimiter(location) override;
	virtual void parser_unclosed_delimiter(location) override;
	virtual void parser_unexpected_delimiter(location) override;
private:
	void print_loc(location);
	void report(location, std::string message);
	void report(location, std::string message, location);
};

#endif //ERRORS_H

