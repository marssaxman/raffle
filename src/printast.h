// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PRINTAST_H
#define PRINTAST_H

#include "ast.h"
#include <iostream>

struct printast: public ast::visitor {
	printast(std::ostream &o): out(o) {}
	virtual void visit(ast::empty&) override;
	virtual void visit(ast::literal&) override;
	virtual void visit(ast::symbol&) override;
	virtual void visit(ast::placeholder&) override;
	virtual void visit(ast::invocation&) override;
	virtual void visit(ast::definition&) override;
	virtual void visit(ast::arithmetic&) override;
	virtual void visit(ast::logic&) override;
	virtual void visit(ast::relation&) override;
	virtual void visit(ast::range&) override;
	virtual void visit(ast::join&) override;
	virtual void visit(ast::sequence&) override;
	virtual void visit(ast::invert&) override;
	virtual void visit(ast::constructor&) override;
private:
	std::ostream &out;
};


#endif //PRINTAST_H

