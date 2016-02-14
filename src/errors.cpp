// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "errors.h"

void errors::report(lexer::position p, std::string message)
{
	std::cerr << p.row << ":" << p.col << ": " << message << std::endl;
}

void errors::lexer_unknown(lexer::position p, char c)
{
	report(p, "unknown character '" + std::string(1, c) + "'");
}

void errors::lexer_nonterminated(lexer::position p)
{
	report(p, "nonterminated string");
}

void errors::parser_unexpected(lexer::position p)
{
	report(p, "syntax error");
}

void errors::parser_missing_operand(lexer::position p)
{
	report(p, "expected expression before this operator");
}

void errors::parser_mismatched_paren(lexer::position p)
{
	report(p, "unmatched parenthesis");
}

void errors::parser_mismatched_bracket(lexer::position p)
{
	report(p, "unmatched bracket");
}

void errors::parser_mismatched_brace(lexer::position p)
{
	report(p, "unmatched brace");
}

void errors::parser_unimplemented(lexer::position p)
{
	report(p, "operator commit implementation missing");
}
