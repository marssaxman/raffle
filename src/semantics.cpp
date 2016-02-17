// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "semantics.h"

using namespace ast;

void semantics::ast_process(ptr &&n) {
	n->accept(*this);
}

void semantics::ast_done() {
}

void semantics::visit(number &n) {
}

void semantics::visit(string &n) {
}

void semantics::visit(symbol &n) {
}

void semantics::visit(wildcard &n) {
}

void semantics::visit(invocation &n) {
}

void semantics::visit(assign &n) {
}

void semantics::visit(capture &n) {
}

void semantics::visit(arithmetic &n) {
}

void semantics::visit(logic &n) {
}

void semantics::visit(relation &n) {
}

void semantics::visit(range &n) {
}

void semantics::visit(invert &n) {
}

void semantics::visit(constructor &n) {
}


