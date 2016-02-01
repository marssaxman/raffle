// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"

#define YY_DEBUG 1
#define YYSTYPE ast::node*
#define YY_CTX_LOCAL
#include "grammar.inl"

ast::node *parse()
{
	yycontext ctx;
	memset(&ctx, 0, sizeof(yycontext));
	yyparse(&ctx);
	yyrelease(&ctx);
	return 0;
}
