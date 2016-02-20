// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "hoas.h"

using namespace hoas;

void lambda::accept(visitor &v) {
	v.visit(*this);
}

void parameter::accept(visitor &v) {
	v.visit(*this);
}


