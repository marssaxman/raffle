// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <iostream>

class lexer {
public:
	struct position {
		unsigned row = 0;
		unsigned col = 0;
	};
	struct output {
		virtual void indent(position, unsigned) = 0;
		virtual void spacer(position) = 0;
		virtual void number(position, std::string) = 0;
		virtual void symbol(position, std::string) = 0;
		virtual void string(position, std::string) = 0;
		virtual void rubric(position, std::string) = 0;
		virtual void opener(position, char) = 0;
		virtual void closer(position, char) = 0;
		virtual void newline(position) = 0;
	};
	struct error {
		virtual void unknown(position, char) = 0;
		virtual void nonterminated(position) = 0;
	};
	lexer(output &o, error &e): out(o), err(e) {}
	void read_line(const std::string&);
	void read_file(std::istream &in);
private:
	position pos;
	output &out;
	error &err;
};

#endif //LEXER_H

