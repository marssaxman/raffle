// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "errors.h"

void errors::report(location l, std::string message)
{
	position p = l.begin;
	std::cerr << p.row << ":" << p.col << ": " << message << std::endl;
}

void errors::lexer_unknown(location l, char c)
{
	report(l, "unknown character '" + std::string(1, c) + "'");
}

void errors::lexer_nonterminated(location l)
{
	report(l, "nonterminated string");
}

void errors::parser_unexpected(location l)
{
	report(l, "syntax error");
}

void errors::parser_missing_operand(location l)
{
	report(l, "expected expression before this operator");
}

void errors::parser_mismatched_paren(location l)
{
	report(l, "unmatched parenthesis");
}

void errors::parser_mismatched_bracket(location l)
{
	report(l, "unmatched bracket");
}

void errors::parser_mismatched_brace(location l)
{
	report(l, "unmatched brace");
}

void errors::parser_unimplemented(location l)
{
	report(l, "operator commit implementation missing");
}
