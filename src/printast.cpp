// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "printast.h"

void printast::visit(ast::empty&) {
}

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
	out << "\xAB";
	n.target->accept(*this);
	switch (n.id) {
		case ast::invocation::subscript: break;
		case ast::invocation::lookup: out << " . "; break;
		case ast::invocation::caption: out << " : "; break;
	}
	n.argument->accept(*this);
	out << "\xBB";
}

void printast::visit(ast::definition& n) {
	out << "\xAB";
	n.sym->accept(*this);
	switch (n.id) {
		case ast::definition::evaluate: out << " <- "; break;
		case ast::definition::capture: out << " -> "; break;
	}
	n.exp->accept(*this);
	out << "\xBB";
}

void printast::visit(ast::arithmetic& n) {
	out << "\xAB";
	n.left->accept(*this);
	switch (n.id) {
		case ast::arithmetic::add: out << " + "; break;
		case ast::arithmetic::subtract: out << " - "; break;
		case ast::arithmetic::multiply: out << " * "; break;
		case ast::arithmetic::divide: out << " / "; break;
		case ast::arithmetic::modulo: out << " % "; break;
		case ast::arithmetic::shift_left: out << " << "; break;
		case ast::arithmetic::shift_right: out << " >> "; break;
	}
	n.right->accept(*this);
	out << "\xBB";
}

void printast::visit(ast::logic& n) {
	out << "\xAB";
	n.left->accept(*this);
	switch (n.id) {
		case ast::logic::conjoin: out << " & "; break;
		case ast::logic::disjoin: out << " | "; break;
		case ast::logic::exclude: out << " ^ "; break;
	}
	n.right->accept(*this);
	out << "\xBB";
}

void printast::visit(ast::relation& n) {
	out << "\xAB";
	n.left->accept(*this);
	switch (n.id) {
		case ast::relation::equal: out << " = "; break;
		case ast::relation::lesser: out << " < "; break;
		case ast::relation::greater: out << " > "; break;
		case ast::relation::not_equal: out << " != "; break;
		case ast::relation::not_lesser: out << " !< "; break;
		case ast::relation::not_greater: out << " !> "; break;
	}
	n.right->accept(*this);
	out << "\xBB";
}

void printast::visit(ast::range& n) {
	out << "\xAB";
	n.left->accept(*this);
	out << "..";
	n.right->accept(*this);
	out << "\xBB";
}

void printast::visit(ast::join& n) {
	out << "\xAB";
	n.exp->accept(*this);
	out << ", ";
	n.next->accept(*this);
	out << "\xBB";
}

void printast::visit(ast::sequence& n) {
	out << "\xAB";
	n.exp->accept(*this);
	out << "; ";
	n.next->accept(*this);
	out << "\xBB";
}

void printast::visit(ast::invert& n) {
	switch (n.id) {
		case ast::invert::negate: out << "-"; break;
		case ast::invert::complement: out << "!"; break;
	}
	n.source->accept(*this);
}

void printast::visit(ast::constructor& n) {
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
}


