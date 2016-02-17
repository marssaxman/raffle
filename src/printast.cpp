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

void printast::visit(invert& n) {
	switch (n.id) {
		case invert::negate: out << "-"; break;
		case invert::complement: out << "!"; break;
	}
	n.source->accept(*this);
}

void printast::visit(ast::tuple &n) {
	infix(*n.left, ",", *n.right);
}

void printast::visit(ast::group& n) {
	unsigned saved = level;
	level = 0;
	switch (n.id) {
		case ast::group::value: out << "("; break;
		case ast::group::construct: out << "["; break;
		case ast::group::scope: out << "{"; break;
	}
	bool pre = false;
	for (auto &i: n.items) {
		if (pre) out << "; ";
		i->accept(*this);
		pre = true;
	}
	switch (n.id) {
		case ast::group::value: out << ")"; break;
		case ast::group::construct: out << "]"; break;
		case ast::group::scope: out << "}"; break;
	}
	level = saved;
}

void printast::infix(binary &n, std::string t) {
	infix(*n.left, t, *n.right);
}

void printast::infix(node &l, std::string t, node &r) {
	if (level++) out << "\xC2\xAB";
	l.accept(*this);
	if (!t.empty()) out << " " << t;
	out << " ";
	r.accept(*this);
	if (--level) out << "\xC2\xBB";
}

