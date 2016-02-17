// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "ast.h"

struct semantics: public ast::delegate, private ast::visitor {
	virtual void ast_process(ast::ptr&&) override;
	virtual void ast_done() override;
private:
	virtual void visit(ast::number&) override;
	virtual void visit(ast::string&) override;
	virtual void visit(ast::symbol&) override;
	virtual void visit(ast::wildcard&) override;
	virtual void visit(ast::apply&) override;
	virtual void visit(ast::assign&) override;
	virtual void visit(ast::capture&) override;
	virtual void visit(ast::arithmetic&) override;
	virtual void visit(ast::logic&) override;
	virtual void visit(ast::relation&) override;
	virtual void visit(ast::range&) override;
	virtual void visit(ast::invert&) override;
	virtual void visit(ast::constructor&) override;
};

#endif //SEMANTICS_H

