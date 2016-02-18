// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "resolver.h"

void resolver::ast_open(ast::group &n) {
	out.ast_open(n);
}

void resolver::ast_process(ast::node &n) {
	out.ast_process(n);
}

void resolver::ast_close(ast::group &n) {
	out.ast_close(n);
}

