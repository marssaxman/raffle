// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "lexer.h"

using std::string;

void lexer::read(const string &input) {
	unsigned row = pos.row();
	unsigned start = pos.col();
	for (auto i = input.begin(); i != input.end(); next(i, input.end())) {
		pos = position(row, start + i - input.begin());
	}
}

void lexer::read_line(const string &input) {
	pos = position(1, 0);
	read(input);
	out.token_eof({pos, pos});
}

void lexer::read_file(std::istream &in) {
	unsigned row = 1;
	for (string line; std::getline(in, line); ++row) {
		pos = position(row, 0);
		read(line);
	}
	out.token_eof({pos, pos});
}

#define SPACE \
	' ': case '\t': case '\f': case '\v'
#define SYMBOL \
	'!': case '$': case '%': case '&': case '*': case '+': case ',': \
	case '-': case '.': case '/': case ':': case ';': case '<': case '=': \
	case '>': case '?': case '@': case '^': case '~': case '|'
#define DIGIT \
	'0': case '1': case '2': case '3': case '4': case '5': case '6': \
	case '7': case '8': case '9'
#define ALPHA \
	'A': case 'a': case 'B': case 'b': case 'C': case 'c': \
	case 'D': case 'd': case 'E': case 'e': case 'F': case 'f': \
	case 'G': case 'g': case 'H': case 'h': case 'I': case 'i': \
	case 'J': case 'j': case 'K': case 'k': case 'L': case 'l': \
	case 'M': case 'm': case 'N': case 'n': case 'O': case 'o': \
	case 'P': case 'p': case 'Q': case 'q': case 'R': case 'r': \
	case 'S': case 's': case 'T': case 't': case 'U': case 'u': \
	case 'V': case 'v': case 'W': case 'w': case 'X': case 'x': \
	case 'Y': case 'y': case 'Z': case 'z'
#define IDENT \
	ALPHA: case DIGIT: case '_'
#define MATCH(X) while(i != end) { X break; }
#define ALL(X) switch(*i) {case X: adv(i, end); continue; default: break;}
#define NOT(X) switch(*i) {case X: break; default: adv(i, end); continue;}
#define UNTIL(X) switch(adv(i, end)) {case X: break; default: continue;}
#define EMIT(X) out.X(string(tokenstart, i), loc)

void lexer::next(string::const_iterator &i, string::const_iterator end) {
	loc.begin = pos;
	auto tokenstart = i;
	switch (adv(i, end)) {
		case SPACE: MATCH(ALL(SPACE)); break;
		case SYMBOL: MATCH(ALL(SYMBOL)); EMIT(token_symbol); break;
		case DIGIT: MATCH(ALL(DIGIT)); EMIT(token_number); break;
		case ALPHA: MATCH(ALL(IDENT)); EMIT(token_identifier); break;
		case '\'': MATCH(UNTIL('\'')); EMIT(token_string); break;
		case '\"': MATCH(UNTIL('\"')); EMIT(token_string); break;
		case '#': MATCH(NOT('\n')); break;
		case '(': out.token_open(token::delim::paren, loc); break;
		case '[': out.token_open(token::delim::bracket, loc); break;
		case '{': out.token_open(token::delim::brace, loc); break;
		case ')': out.token_close(token::delim::paren, loc); break;
		case ']': out.token_close(token::delim::bracket, loc); break;
		case '}': out.token_close(token::delim::brace, loc); break;
		case '_': out.token_underscore(loc); break;
		default: {
			string msg;
			char c = *tokenstart;
			if (isprint(c)) {
				msg = string(1, c);
			} else {
				static const char xdig[] = "0123456789ABCDEF";
				char hex[5] {'\\', 'x', xdig[(c >> 4) & 0x0F], xdig[c & 0x0F]};
				msg = string(hex);
			}
			err.report(loc, "unexpected character '" + msg + "'");
		} break;
	}
}

char lexer::adv(string::const_iterator &i, string::const_iterator end) {
	char c = 0;
	if (i != end) {
		c = *i;
		pos = position(pos.row(), pos.col() + 1);
		loc.end = pos;
		++i;
	}
	return c;
}

void lexer::ret(string::const_iterator &i, string::const_iterator end) {
	if (pos.col() > 1) {
		pos = position(pos.row(), pos.col() - 1);
		loc.end = pos;
	}
	--i;
}
