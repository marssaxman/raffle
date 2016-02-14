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
	virtual void lexer_unknown(lexer::position p, char c) override;
	virtual void lexer_nonterminated(lexer::position p) override;
	virtual void parser_unexpected(lexer::position) override;
	virtual void parser_missing_operand(lexer::position) override;
	virtual void parser_mismatched_paren(lexer::position) override;
	virtual void parser_mismatched_bracket(lexer::position) override;
	virtual void parser_mismatched_brace(lexer::position) override;
	virtual void parser_unimplemented(lexer::position) override;
private:
	void report(lexer::position, std::string message);
};

#endif //ERRORS_H

