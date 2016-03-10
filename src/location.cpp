// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "location.h"

static_assert(4 == sizeof(position),
		"position must stay small because it is used so frequently");
static_assert(8 <= sizeof(location),
		"location must be 8 bytes or less for efficient parameter passing");

position::position(unsigned r, unsigned c) {
	value = r << 8;
	if (c <= 0xFF) {
		value |= c;
	}
}

bool position::operator<(const position &other) const {
	if (row() < other.row()) return true;
	if (row() > other.row()) return false;
	return col() < other.col();
}

location::location(position b, position e): begin(b), end(e) {
	if (e < b) {
		begin = e;
		end = b;
	}
}

location location::span(position b, unsigned n) {
	return location(b, position(b.row(), b.col() + n));
}

location location::operator+(const location &other) const {
	location out = *this;
	if (other.begin < begin) {
		out.begin = other.begin;
	}
	if (other.end > end) {
		out.end = other.end;
	}
	return out;
}


