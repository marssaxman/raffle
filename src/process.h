// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PROCESS_H
#define PROCESS_H

template <typename... L>
struct visitor;

template <typename T>
struct visitor<T> {
	virtual void operator()(T) = 0;
};

template <typename T, typename... L>
struct visitor<T, L...> : visitor<L...> {
	using visitor<L...>::operator();
	virtual void operator()(T) = 0;
};

#endif //PROCESS_H

