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
		virtual void token_number(position, std::string) = 0;
		virtual void token_symbol(position, std::string) = 0;
		virtual void token_string(position, std::string) = 0;
		virtual void token_paren_open(position) = 0;
		virtual void token_paren_close(position) = 0;
		virtual void token_bracket_open(position) = 0;
		virtual void token_bracket_close(position) = 0;
		virtual void token_brace_open(position) = 0;
		virtual void token_brace_close(position) = 0;
		virtual void token_comma(position) = 0;
		virtual void token_semicolon(position) = 0;
	};
	struct error {
		virtual void token_unknown(position, char) = 0;
		virtual void token_nonterminated(position) = 0;
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

