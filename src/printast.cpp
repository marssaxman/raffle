// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "printast.h"

using namespace ast;

void printast::visit(ast::literal& n) {
	out << n.text;
}

void printast::visit(ast::symbol& n) {
	out << n.text;
}

void printast::visit(ast::placeholder& n) {
	out << "_";
}

void printast::visit(ast::invocation& n) {
	switch (n.id) {
		case invocation::subscript:
			seq(*n.target.get(), "", *n.argument.get());
			break;
		case invocation::lookup:
			seq(*n.target.get(), ".", *n.argument.get());
			break;
		case invocation::caption:
			infix(*n.target, ":", *n.argument);
			break;
	}
}

void printast::visit(ast::definition& n) {
	switch (n.id) {
		case definition::evaluate: infix(*n.sym, "<-", *n.exp); break;
		case definition::capture: infix(*n.sym, "->", *n.exp); break;
	}
}

void printast::visit(ast::arithmetic& n) {
	switch (n.id) {
		case arithmetic::add: infix(n, "+"); break;
		case arithmetic::subtract: infix(n, "-"); break;
		case arithmetic::multiply: infix(n, "*"); break;
		case arithmetic::divide: infix(n, "/"); break;
		case arithmetic::modulo: infix(n, "%"); break;
		case arithmetic::shift_left: infix(n, "<<"); break;
		case arithmetic::shift_right: infix(n, ">>"); break;
	}
}

void printast::visit(ast::logic& n) {
	switch (n.id) {
		case logic::conjoin: infix(n, "&"); break;
		case logic::disjoin: infix(n, "|"); break;
		case logic::exclude: infix(n, "^"); break;
	}
}

void printast::visit(ast::relation& n) {
	switch (n.id) {
		case relation::equal: infix(n, "="); break;
		case relation::lesser: infix(n, "<"); break;
		case relation::greater: infix(n, ">"); break;
		case relation::not_equal: infix(n, "!="); break;
		case relation::not_lesser: infix(n, "!<"); break;
		case relation::not_greater: infix(n, "!>"); break;
	}
}

void printast::visit(ast::range& n) {
	infix(n, "..");
}

void printast::visit(ast::join& n) {
	seq(*n.exp, ", ", *n.next);
}

void printast::visit(ast::sequence& n) {
	seq(*n.exp, "; ", *n.next);
}

void printast::visit(invert& n) {
	switch (n.id) {
		case invert::negate: out << "-"; break;
		case invert::complement: out << "!"; break;
	}
	n.source->accept(*this);
}

void printast::visit(ast::constructor& n) {
	unsigned saved = level;
	level = 0;
	switch (n.id) {
		case ast::constructor::tuple: out << "("; break;
		case ast::constructor::list: out << "["; break;
		case ast::constructor::object: out << "{"; break;
	}
	n.items->accept(*this);
	switch (n.id) {
		case ast::constructor::tuple: out << ")"; break;
		case ast::constructor::list: out << "]"; break;
		case ast::constructor::object: out << "}"; break;
	}
	level = saved;
}

void printast::visit(ast::empty &n) {
	switch (n.id) {
		case empty::tuple: out << "()"; break;
		case empty::list: out << "[]"; break;
		case empty::object: out << "{}"; break;
	}
}

void printast::infix(binary &n, std::string t) {
	infix(*n.left, t, *n.right);
}

void printast::infix(node &l, std::string t, node &r) {
	if (level++) out << "\xC2\xAB";
	seq(l, " " + t + " ", r);
	if (--level) out << "\xC2\xBB";
}

void printast::seq(node &l, std::string t, node &r) {
	l.accept(*this);
	out << t;
	r.accept(*this);
}