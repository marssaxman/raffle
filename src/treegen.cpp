// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "treegen.h"

void treegen::emit_eof(location origin) {
	store(new ast::eof(origin));
}

void treegen::emit_wildcard(location origin) {
	store(new ast::wildcard(origin));
}

void treegen::emit_null(location origin) {
	store(new ast::null(origin));
}

void treegen::emit_number(std::string text, location origin) {
	store(new ast::number(text, origin));
}

void treegen::emit_string(std::string text, location origin) {
	store(new ast::string(text, origin));
}

void treegen::emit_identifier(std::string text, location origin) {
	store(new ast::identifier(text, origin));
}

void treegen::emit_branch(syntax::branch id, std::string text, location o) {
	ast::unique_ptr right = recall();
	ast::unique_ptr left = recall();
	switch (id) {
		case syntax::apply:
			store(new ast::apply(std::move(left), std::move(right), o));
			break;
		case syntax::pipe:
			store(new ast::pipe(std::move(left), std::move(right), o));
			break;
		case syntax::sequence:
			store(new ast::sequence(std::move(left), std::move(right), o));
			break;
		case syntax::pair:
			store(new ast::pair(std::move(left), std::move(right), o));
			break;
		case syntax::range:
			store(new ast::range(std::move(left), std::move(right), o));
			break;
		case syntax::assign:
			store(new ast::assign(std::move(left), std::move(right), o));
			break;
		case syntax::capture:
			store(new ast::capture(std::move(left), std::move(right), o));
			break;
		case syntax::declare:
			store(new ast::declare(std::move(left), std::move(right), o));
			break;
		case syntax::define:
			store(new ast::define(std::move(left), std::move(right), o));
			break;
		case syntax::typealias:
			store(new ast::typealias(std::move(left), std::move(right), o));
			break;
		default:
			store(new ast::binop(text, std::move(left), std::move(right), o));
			break;
	}
}

void treegen::store(ast::node *n) {
	ast::unique_ptr p(n);
	out.process(std::move(p));
	state.emplace(std::move(p));
}

ast::unique_ptr &&treegen::recall() {
	ast::unique_ptr out = std::move(state.top());
	state.pop();
	return std::move(out);
}
