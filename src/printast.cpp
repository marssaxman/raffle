// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "printast.h"

using namespace ast;

void printast::visit(ast::number& n) {
	out << n.text;
}

void printast::visit(ast::string& n) {
	out << "\"" << n.text << "\"";
}

void printast::visit(ast::symbol& n) {
	out << n.text;
}

void printast::visit(ast::wildcard& n) {
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
	std::string sep;
	std::string close;
	switch (n.id) {
		case ast::constructor::tuple:
			out << "("; sep = ", "; close = ")"; break;
		case ast::constructor::list:
			out << "["; sep = ", "; close = "]"; break;
		case ast::constructor::object:
			out << "{"; sep = "; "; close = "}"; break;
	}
	bool pre = false;
	for (auto &i: n.items) {
		if (pre) out << sep;
		i->accept(*this);
		pre = true;
	}
	out << close;
	level = saved;
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
