// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef AST_H
#define AST_H

namespace ast
{

struct node
{
};

struct leaf: public node
{
};

struct number: public leaf
{

};

struct identifier: public leaf
{
};

struct procedure: public node
{
};

struct definition: public node
{
};

struct function: public node
{
};

struct list: public node
{
};

class comparison: public node
{
};

class equal: public comparison
{
};

class greater: public comparison
{
};

class lesser: public comparison
{
};

class arithmetic: public node
{
};

class add: public arithmetic
{
};

class subtract: public arithmetic
{
};

class multiply: public arithmetic
{
};

class divide: public arithmetic
{
};

class modulo: public arithmetic
{
};

class shiftleft: public arithmetic
{
};

class shiftright: public arithmetic
{
};



} // namespace ast

#endif //AST_H
