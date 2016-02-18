// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef DFG_H
#define DFG_H

#include "type.h"
#include "location.h"

namespace dfg {

struct visitor;

struct node {
	node(location l, type::id t): loc(l), type(t) {}
	node(type::id t): loc(), type(t) {}
	location loc;
	std::string type;
	virtual void accept(visitor&) = 0;
};

struct failure: public node {
	failure(location l): node(l, type::failure) {}
	virtual void accept(visitor&);
};

struct integer: public node {
	integer(location l, type::id t, int64_t v): node(l, t), value(v) {}
	integer(location l, int64_t v): node(l, type::integer), value(v) {}
	int64_t value;
	virtual void accept(visitor&);
};

struct string: public node {
	string(location l, type::id t, std::string v): node(l, t), value(v) {}
	string(location l, std::string v): node(l, type::string), value(v) {}
	std::string value;
	virtual void accept(visitor&);
};

struct builtin: public node {
	builtin(type::id t, size_t i): node(t), index(i) {}
	size_t index;
	virtual void accept(visitor&);
};

struct parameter: public node {
	parameter(location l, type::id t): node(l, t)  {}
	virtual void accept(visitor&);
};

struct closure: public node {
	closure(location l, type::id t): node(l, t) {}
	virtual void accept(visitor&);
};

struct capture: public node {
	capture(location l, type::id t, node &b, node &c);
	node &body;
	node &context;
	virtual void accept(visitor&);
};

struct apply: public node {
	apply(location l, type::id t, node &f, node &a);
	node &function;
	node &arg;
	virtual void accept(visitor&);
};

struct visitor {
	virtual void visit(failure&) = 0;
	virtual void visit(integer&) = 0;
	virtual void visit(string&) = 0;
	virtual void visit(builtin&) = 0;
	virtual void visit(parameter&) = 0;
	virtual void visit(closure&) = 0;
	virtual void visit(capture&) = 0;
	virtual void visit(apply&) = 0;
};

} // namespace dfg

#endif //DFG_H


