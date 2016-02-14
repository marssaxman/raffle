// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "location.h"

bool position::operator<(const position &other) const
{
	if (row < other.row) return true;
	if (row > other.row) return false;
	return col < other.col;
}

location location::operator+(const location &other) const
{
	location out = *this;
	if (other.begin < begin) {
		out.begin = other.begin;
	}
	if (other.end > end) {
		out.end = other.end;
	}
	return out;
}


