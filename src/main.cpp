// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include <iostream>
#include <fstream>

#define YY_DEBUG 1
#include "grammar.inl"

int main(int argc, const char *argv[])
{
	return yyparse()? EXIT_SUCCESS: EXIT_FAILURE;
}

