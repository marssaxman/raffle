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

void errors::token_unknown(lexer::position p, char c)
{
	report(p, "unknown character '" + std::string(1, c) + "'");
}

void errors::token_nonterminated(lexer::position p)
{
	report(p, "nonterminated string");
}

void errors::parse_unexpected(lexer::position p)
{
	report(p, "syntax error");
}

void errors::parse_mismatched_paren(lexer::position p)
{
	report(p, "unmatched parenthesis");
}

void errors::parse_mismatched_bracket(lexer::position p)
{
	report(p, "unmatched bracket");
}

void errors::parse_mismatched_brace(lexer::position p)
{
	report(p, "unmatched brace");
}

