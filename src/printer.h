// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PRINTER_H
#define PRINTER_H

#include "ast.h"
#include <iostream>

struct printer: public ast::visitor {
	printer(std::ostream &o): out(o) {}
	virtual void visit(ast::number&) override;
	virtual void visit(ast::string&) override;
	virtual void visit(ast::symbol&) override;
	virtual void visit(ast::wildcard&) override;
	virtual void visit(ast::apply &n) override { infix(n, ""); }
	virtual void visit(ast::pipeline &n) override { infix(n, "."); }
	virtual void visit(ast::assign &n) override { infix(n, "<-"); }
	virtual void visit(ast::capture &n) override { infix(n, "->"); }
	virtual void visit(ast::define &n) override { infix(n, ":"); }
	virtual void visit(ast::operate&) override;
	virtual void visit(ast::range &n) override { infix(n, ".."); }
	virtual void visit(ast::negate&) override;
	virtual void visit(ast::group&) override;
private:
	void infix(ast::node &l, std::string, ast::node &r);
	void infix(ast::binary&, std::string);
	unsigned level = 0;
	std::ostream &out;
};

#endif //PRINTER_H

