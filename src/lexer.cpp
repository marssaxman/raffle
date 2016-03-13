// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "lexer.h"

#define SPACE \
	' ': case '\t': case '\f': case '\v'
#define DELIM \
	'(': case ')': case '[': case ']': case '{': case '}': case ';': case ','
#define SYMBOL \
	'!': case '$': case '%': case '&': case '*': case '+': case '-': \
	case '.': case '/': case ':': case '<': case '=': case '>': case '?': \
	case '@': case '^': case '~': case '|'
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
#define IDSTART \
	ALPHA: case '_'
#define IDBODY \
	IDSTART: case DIGIT

void lexer::read(char c) {
	switch (state) {
		case start: switch (c) {
			case '#': accept(c); state = comment; break;
			case DIGIT: accept(c); state = number; break;
			case '\"': accept(c); state = string; break;
			case IDSTART: accept(c); state = identifier; break;
			case SYMBOL: accept(c); state = symbol; break;
			case DELIM: accept(c); emit<token::delimiter>(); break;
			case '\n': tk_end = tk_end.next_row(); clear(); break;
			case SPACE: accept(c); state = space; break;
			case 0: clear(); state = eof; break;
			default: reject(c); break;
		} break;

		case comment: switch (c) {
			case '\n': clear(); read(c); break;
			default: accept(c); break;
		} break;

		case number: switch (c) {
			case DIGIT: accept(c); break;
			default: emit<token::number>(); read(c); break;
		} break;

		case string: switch (c) {
			case '\"': accept(c); emit<token::string>(); break;
			default: accept(c); break;
		} break;

		case identifier: switch (c) {
			case IDBODY: accept(c); break;
			default: emit<token::identifier>(); read(c); break;
		} break;

		case symbol: switch (c) {
			case SYMBOL: accept(c); break;
			default: emit<token::symbol>(); read(c); break;
		} break;

		case space: switch (c) {
			case SPACE: accept(c); break;
			default: clear(); read(c); break;
		} break;

		case eof: switch (c) {
			reject(c); break;
		} break;
	}
}

void lexer::accept(char c) {
	buf << c;
	tk_end = tk_end.next_col();
}

void lexer::reject(char c) {
	std::string msg;
	if (isprint(c)) {
		msg = std::string(1, c);
	} else {
		static const char xdig[] = "0123456789ABCDEF";
		char hex[5] {'\\', 'x', xdig[(c >> 4) & 0x0F], xdig[c & 0x0F]};
		msg = std::string(hex);
	}
	location loc(tk_begin, tk_end);
	err.report(loc, "unexpected character '" + msg + "'");
	clear();
}

void lexer::clear() {
	buf.clear();
	buf.str("");
	tk_begin = tk_end;
	state = start;
}

