// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PROCESS_H
#define PROCESS_H

template <typename... L>
struct dsl;

template <typename T>
struct dsl<T> {
	struct ostream {
		virtual void flush() = 0;
		virtual void operator<<(T) = 0;
	};

	template <typename V>
	struct visitor {
		virtual void operator()(V, T) = 0;
	};
};

template <typename T, typename... L>
struct dsl<T, L...>: dsl<L...> {
	struct ostream: dsl<L...>::ostream {
		using dsl<L...>::ostream::operator<<;
		virtual void operator<<(T) = 0;
	};

	template<typename V>
	struct visitor: dsl<L...>::visitor {
		using dsl<L...>::visitor::operator();
		virtual void operator()(V, T) = 0;
	};
};

#endif //PROCESS_H

