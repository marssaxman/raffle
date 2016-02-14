// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef LOCATION_H
#define LOCATION_H

struct position {
	unsigned row = 0;
	unsigned col = 0;
	bool operator<(const position&) const;
	bool operator>(const position &other) const { return !(*this < other); }
};

struct location {
	position begin;
	position end;
	location() {}
	location(position b, position e): begin(b), end(e) {}
	location operator+(const location&) const;
};

#endif //LOCATION_H

