// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "dfg.h"

using namespace dfg;

void failure::accept(visitor &v) {
	v.visit(*this);
}

void integer::accept(visitor &v) {
	v.visit(*this);
}

void string::accept(visitor &v) {
	v.visit(*this);
}

void builtin::accept(visitor &v) {
	v.visit(*this);
}

void parameter::accept(visitor &v) {
	v.visit(*this);
}

void closure::accept(visitor &v) {
	v.visit(*this);
}

capture::capture(location l, type::id t, node &b, node &c):
		node(l, t), body(b), context(c) {
}

void capture::accept(visitor &v) {
	v.visit(*this);
}

apply::apply(location l, type::id t, node &f, node &a):
		node(l, t), function(f), arg(a) {
}

void apply::accept(visitor &v) {
	v.visit(*this);
}

