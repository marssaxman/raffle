// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef PIPELINE_H
#define PIPELINE_H

#include <deque>
#include <iostream>

class pipe
{
public:
	virtual bool run() = 0;
};

// a port from which data comes
template<typename T>
class source
{
public:
	virtual bool ready() = 0;
	virtual T produce() = 0;
};

// a port into which data goes
template<typename T>
class sink
{
public:
	virtual bool clear() = 0;
	virtual void consume(T) = 0;
};

// a reducer combines source items into a smaller number of outputs
template<typename I, typename O>
class reducer: public source<O>
{
	source<I> &input;
public:
	reducer(source<I> &in): input(in) {}
};

// an expander produces more outputs from its source items
template <typename I, typename O>
class expander: public sink<O>
{
	sink<O> &output;
public:
	expander(sink<O> &out): output(out) {}
};

// a fitting is a pipe which joins an output and an input
template<typename T>
class fitting: public pipe
{
	source<T> &input;
	sink<T> &output;
public:
	fitting(source<T> &in, sink<T> &out): input(in), output(out) {}
	bool run() override;
};

// a filter is a pipe that changes one type of data into another
template<typename I, typename O>
class filter: public pipe
{
	source<I> &input;
	sink<O> &output;
public:
	filter(source<I> &in, sink<O> &out): input(in), output(out) {}
	bool run() override;
protected:
	virtual O apply(I) = 0;
};

// buffers allow decoupling of sources from sinks, so you can connect an
// expander to a reducer
template<typename T>
class buffer: public source<T>, sink<T>
{
public:
	bool ready() override { return !q.empty(); }
	T produce() override;
	bool clear() override { return true; }
	void consume(T val) override { q.push_front(val); }
private:
	std::deque<T> q;
};

#endif //PIPELINE_H
