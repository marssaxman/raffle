// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef LOCATION_H
#define LOCATION_H

#include <stdint.h>

struct position {
	position() {}
	position(unsigned r, unsigned c);
	unsigned row() const { return value >> 8; }
	unsigned col() const { return value & 0xFF; }
	bool operator<(const position&) const;
	bool operator>(const position &other) const { return !(*this < other); }
	position next_col() const { return position(value + !!(value ^ 0xFF)); }
	position next_row() const { return position((value | 0xFF) + 1); }
private:
	position(uint32_t v): value(v) {}
	uint32_t value = 0;
};

struct location {
	position begin;
	position end;
	location() {}
	location(position b, position e);
	static location span(position, unsigned);
	location operator+(const location&) const;
};

#endif //LOCATION_H

