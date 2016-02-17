// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "ast.h"
#include <assert.h>

using namespace ast;

void number::accept(visitor &v) {
	v.visit(*this);
}

void string::accept(visitor &v) {
	v.visit(*this);
}

symbol::symbol(std::string t, location l):
		text(t), tk_loc(l) {
}

location symbol::loc() {
	return tk_loc;
}

void symbol::accept(visitor &v) {
	v.visit(*this);
}

void wildcard::accept(visitor &v) {
	v.visit(*this);
}

apply::apply(ptr &&t, ptr &&a):
		binary(std::move(t), std::move(a)) {
}

void apply::accept(visitor &v) {
	v.visit(*this);
}

compose::compose(ptr &&a, ptr &&t):
		binary(std::move(a), std::move(t)) {
}

void compose::accept(visitor &v) {
	v.visit(*this);
}

assign::assign(ptr &&s, ptr &&e):
		binary(std::move(s), std::move(e)) {
}

void assign::accept(visitor &v) {
	v.visit(*this);
}

capture::capture(ptr &&s, ptr &&e):
		binary(std::move(s), std::move(e)) {
}

void capture::accept(visitor &v) {
	v.visit(*this);
}

define::define(ptr &&s, ptr &&e):
		binary(std::move(s), std::move(e)) {
}

void define::accept(visitor &v) {
	v.visit(*this);
}

binary::binary(ptr &&l, ptr &&r):
		left(std::move(l)), right(std::move(r)) {
	assert(left);
	assert(right);
}

arithmetic::arithmetic(opcode o, ptr &&l, ptr &&r):
		binary(std::move(l), std::move(r)), id(o) {
}

void arithmetic::accept(visitor &v) {
	v.visit(*this);
}

logic::logic(opcode o, ptr &&l, ptr &&r):
		binary(std::move(l), std::move(r)), id(o) {
}

void logic::accept(visitor &v) {
	v.visit(*this);
}

relation::relation(opcode o, ptr &&l, ptr &&r):
		binary(std::move(l), std::move(r)), id(o) {
}

void relation::accept(visitor &v) {
	v.visit(*this);
}

range::range(ptr &&l, ptr &&r):
		binary(std::move(l), std::move(r)) {
}

void range::accept(visitor &v) {
	v.visit(*this);
}

invert::invert(opcode o, ptr &&s, location l):
		id(o), source(std::move(s)), tk_loc(l) {
}

void invert::accept(visitor &v) {
	v.visit(*this);
}

tuple::tuple(ptr &&l, ptr &&r):
		binary(std::move(l), std::move(r)) {
}

void tuple::accept(visitor &v) {
	v.visit(*this);
}

constructor::constructor(opcode o, std::list<ptr> &&i, location l):
		id(o), items(std::move(i)), tk_loc(l) {
}

void constructor::accept(visitor &v) {
	v.visit(*this);
}

