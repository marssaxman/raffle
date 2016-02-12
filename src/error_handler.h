// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "lexer.h"
#include "parser.h"

struct error_handler: public lexer::error, public parser::error {
	virtual void token_unknown(lexer::position p, char c) override;
	virtual void token_nonterminated(lexer::position p) override;
	virtual void parse_expect_paren(lexer::position) override;
	virtual void parse_expect_bracket(lexer::position) override;
	virtual void parse_expect_brace(lexer::position) override;
	virtual void parse_unexpected_close(lexer::position) override;
private:
	void report(lexer::position, std::string message);
};

#endif //ERROR_HANDLER_H

