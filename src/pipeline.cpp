// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "pipeline.h"

template<typename T>
bool fitting<T>::run()
{
	bool did_work = false;
	while (input.ready() && output.clear()) {
		output.consume(input.produce());
		did_work = true;
	}
	return did_work;
}

template<typename T>
T buffer<T>::produce()
{
	T val();
	if (!q.empty()) {
		val = q.back();
		q.pop_back();
	}
	return val;
}


