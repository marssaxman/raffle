// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "printer.h"
#include <sstream>
#include <map>

using namespace ast;

void printer::visit(ast::number& n) {
	out << n.text;
}

void printer::visit(ast::string& n) {
	out << "\"" << n.text << "\"";
}

void printer::visit(ast::symbol& n) {
	out << n.text;
}

void printer::visit(ast::wildcard& n) {
	out << "_";
}

void printer::visit(ast::binary& n) {
	static std::map<ast::binary::opcode, std::string> labels = {
		{binary::opcode::add, " + "},
		{binary::opcode::sub, " - "},
		{binary::opcode::mul, " * "},
		{binary::opcode::div, " / "},
		{binary::opcode::rem, " % "},
		{binary::opcode::shl, " << "},
		{binary::opcode::shr, " >> "},
		{binary::opcode::conjoin, " & "},
		{binary::opcode::disjoin, " | "},
		{binary::opcode::exclude, " ^ "},
		{binary::opcode::eq, " = "},
		{binary::opcode::lt, " < "},
		{binary::opcode::gt, " > "},
		{binary::opcode::neq, " != "},
		{binary::opcode::nlt, " !< "},
		{binary::opcode::ngt, " !> "},
		{binary::opcode::apply, ""},
		{binary::opcode::pipeline, "."},
		{binary::opcode::assign, " <- "},
		{binary::opcode::capture, " -> "},
		{binary::opcode::declare, " := "},
		{binary::opcode::define, ": "},
		{binary::opcode::typealias, " ::= "},
		{binary::opcode::range, ".."},
		{binary::opcode::sequence, "; "},
		{binary::opcode::tuple, ", "},
	};
	auto iter = labels.find(n.id);
	std::string t;
	if (iter != labels.end()) {
		t = iter->second;
	} else {
		t = " ?" + std::to_string((int)n.id) + "? ";
	}
	if (level++) out << "\xC2\xAB";
	n.left->accept(*this);
	out << t;
	n.right->accept(*this);
	if (--level) out << "\xC2\xBB";
}

void printer::group(std::string l, ast::group& n, std::string r) {
	out << l;
	printer sub(out);
	n.source->accept(sub);
	out << r;
}

