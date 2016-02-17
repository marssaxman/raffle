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

void printast::visit(ast::operate& n) {
	switch (n.id) {
		case operate::add: infix(n, "+"); break;
		case operate::sub: infix(n, "-"); break;
		case operate::mul: infix(n, "*"); break;
		case operate::div: infix(n, "/"); break;
		case operate::rem: infix(n, "%"); break;
		case operate::shl: infix(n, "<<"); break;
		case operate::shr: infix(n, ">>"); break;
		case operate::conjoin: infix(n, "&"); break;
		case operate::disjoin: infix(n, "|"); break;
		case operate::exclude: infix(n, "^"); break;
		case operate::eq: infix(n, "="); break;
		case operate::lt: infix(n, "<"); break;
		case operate::gt: infix(n, ">"); break;
		case operate::neq: infix(n, "!="); break;
		case operate::nlt: infix(n, "!<"); break;
		case operate::ngt: infix(n, "!>"); break;
	}
}

void printast::visit(negate& n) {
	switch (n.id) {
		case negate::numeric: out << "-"; break;
		case negate::logical: out << "!"; break;
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

