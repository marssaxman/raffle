// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PROCESS_H
#define PROCESS_H

template <typename... L>
struct output;

template <typename T>
struct output<T> {
	virtual void flush() = 0;
	virtual void operator<<(T) = 0;
};

template <typename T, typename... L>
struct output<T, L...> : output<L...> {
	using output<L...>::operator<<;
	virtual void operator<<(T) = 0;
};


template <typename V, typename... L>
struct visitor;

template <typename V, typename T>
struct visitor<V, T> {
	virtual void operator()(V, T) = 0;
};

template <typename V, typename T, typename... L>
struct visitor<V, T, L...>: visitor<V, L...> {
	using visitor<V, L...>::operator();
	virtual void operator()(V, T) = 0;
};


template <typename... L>
struct dsl;

template <typename T>
struct dsl<T> {
	using output = ::output<T>;

	template<typename V>
	using visitor = ::visitor<V, T>;
};

template <typename T, typename... L>
struct dsl<T, L...> {
	using output = ::output<T, L...>;

	template<typename V>
	using visitor = ::visitor<V, T, L...>;
};



#endif //PROCESS_H

