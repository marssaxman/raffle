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
	virtual void parser_missing_left_operand(location) override;
	virtual void parser_missing_right_operand(location) override;
	virtual void parser_mismatched_group(location) override;
	virtual void parser_mismatched_separator(location) override;
private:
	void report(location, std::string message);
};

#endif //ERRORS_H

