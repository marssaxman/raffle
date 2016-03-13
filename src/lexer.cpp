// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "lexer.h"

using std::string;

void lexer::read(const string &input) {
	tokenstart = input.begin();
	i = input.begin();
	end = input.end();
	while (i != input.end()) {
		next();
		tokenstart = i;
	}
}

void lexer::read_line(const string &input) {
	pos = position(1, 0);
	read(input);
	out.token_eof(current());
}

void lexer::read_file(std::istream &in) {
	unsigned row = 1;
	for (string line; std::getline(in, line); ++row) {
		pos = position(row, 0);
		read(line);
	}
	out.token_eof(current());
}

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
#define STRING \
	'\"': case '\''

#define MATCH(X) while(i != end) { X break; }
#define ALL(X) switch(*i) {case X: ++i; continue; default: break;}
#define NOT(X) switch(*i) {case X: break; default: ++i; continue;}
#define UNTIL(X) switch(*i++) {case X: break; default: continue;}
#define EMIT(X) out.X(current())

void lexer::next() {
	if (i != end) switch (*i++) {
		case SPACE: MATCH(ALL(SPACE)); break;
		case DELIM: EMIT(token_delimiter); break;
		case SYMBOL: MATCH(ALL(SYMBOL)); EMIT(token_symbol); break;
		case DIGIT: MATCH(ALL(DIGIT)); EMIT(token_number); break;
		case IDSTART: MATCH(ALL(IDBODY)); EMIT(token_identifier); break;
		case '\"': MATCH(UNTIL('\"')); EMIT(token_string); break;
		case '#': MATCH(NOT('\n')); break;
		default: {
			char c = *tokenstart;
			std::string msg;
			if (isprint(c)) {
				msg = string(1, c);
			} else {
				static const char xdig[] = "0123456789ABCDEF";
				char hex[5] {'\\', 'x', xdig[(c >> 4) & 0x0F], xdig[c & 0x0F]};
				msg = string(hex);
			}
			location errloc = location::span(pos, 1);
			err.report(errloc, "unexpected character '" + msg + "'");
		} break;
	}
	pos = position(pos.row(), pos.col() + i - tokenstart);
}

token lexer::current() const {
	size_t len = i - tokenstart;
	return token(std::string(tokenstart, i), location::span(pos, len));
}

