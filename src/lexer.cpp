// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "lexer.h"

using std::string;

void lexer::read(const string &input) {
	unsigned start = pos.col;
	for (auto i = input.begin(); i != input.end(); next(i, input.end())) {
		pos.col = start + i - input.begin();
	}
}

void lexer::read_line(const string &input) {
	pos.col = 1;
	read(input);
	out.token_eof({pos, pos});
}

void lexer::read_file(std::istream &in) {
	pos.row = 1;
	for (string line; std::getline(in, line); ++pos.row) {
		pos.col = 1;
		read(line);
	}
	out.token_eof({pos, pos});
}

void lexer::next(string::const_iterator &i, string::const_iterator end) {
	loc.begin = pos;
	auto tokenstart = i;
	switch (adv(i, end)) {
		case '#':
			// Comment extends to end of line
			i = end;
			break;

		case '(': out.token_l_paren(loc); break;
		case ')': out.token_r_paren(loc); break;
		case '[': out.token_l_bracket(loc); break;
		case ']': out.token_r_bracket(loc); break;
		case '{': out.token_l_brace(loc); break;
		case '}': out.token_r_brace(loc); break;

		case ':': switch (adv(i, end)) {
			case '=': out.token_colon_equal(loc); break;
			case ':': switch (adv(i, end)) {
				case '=':out.token_double_colon_equal(loc); break;
				default: ret(i, end);
			} break;
			default: ret(i, end); out.token_colon(loc);
		} break;

		case ',': out.token_comma(loc); break;
		case ';': out.token_semicolon(loc); break;
		case '+': out.token_plus(loc); break;
		case '*': out.token_star(loc); break;
		case '/': out.token_slash(loc); break;
		case '%': out.token_percent(loc); break;
		case '=': out.token_equal(loc); break;
		case '&': out.token_ampersand(loc); break;
		case '|': out.token_pipe(loc); break;
		case '^': out.token_caret(loc); break;
		case '~': out.token_tilde(loc); break;

		case '!': switch (adv(i, end)) {
			case '=': out.token_bang_equal(loc); break;
			case '<': out.token_l_bangle(loc); break;
			case '>': out.token_r_bangle(loc); break;
			case '~': out.token_bang_tilde(loc); break;
			default: ret(i, end); out.token_bang(loc);
		} break;

		case '.': switch (adv(i, end)) {
			case '.': out.token_dot_dot(loc); break;
			default: ret(i, end); out.token_dot(loc);
		} break;

		case '<': switch (adv(i, end)) {
			case '-': out.token_l_arrow(loc); break;
			case '<': out.token_l_guillemet(loc); break;
			default: ret(i, end); out.token_l_angle(loc);
		} break;

		case '>': switch (adv(i, end)) {
			case '>': out.token_r_guillemet(loc); break;
			default: ret(i, end); out.token_r_angle(loc);
		} break;

		case '-': switch (adv(i, end)) {
			case '>': out.token_r_arrow(loc); break;
			default: ret(i, end); out.token_hyphen(loc);
		} break;

		case '\"':
		case '\'':
			for (char delim = *tokenstart++; i != end; adv(i, end)) {
				if (*i == delim) {
					string text(tokenstart, i);
					adv(i, end);
					out.token_string(loc, text);
					break;
				}
			}
			if (i == end) {
				err.lexer_nonterminated(loc);
			}
			break;

		case ' ':
		case '\t':
		case '\f':
		case '\v':
			break;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			while (isdigit(adv(i, end))) {}
			out.token_number(loc, string(tokenstart, i));
			break;

		case '_':
			out.token_underscore(loc);
			break;

		case 'A': case 'a':
		case 'B': case 'b':
		case 'C': case 'c':
		case 'D': case 'd':
		case 'E': case 'e':
		case 'F': case 'f':
		case 'G': case 'g':
		case 'H': case 'h':
		case 'I': case 'i':
		case 'J': case 'j':
		case 'K': case 'k':
		case 'L': case 'l':
		case 'M': case 'm':
		case 'N': case 'n':
		case 'O': case 'o':
		case 'P': case 'p':
		case 'Q': case 'q':
		case 'R': case 'r':
		case 'S': case 's':
		case 'T': case 't':
		case 'U': case 'u':
		case 'V': case 'v':
		case 'W': case 'w':
		case 'X': case 'x':
		case 'Y': case 'y':
		case 'Z': case 'z':
			while (i != end && isalnum(*i) || '_' == *i) {
				adv(i, end);
			}
			out.token_symbol(loc, string(tokenstart, i));
			break;

		default:
			err.lexer_unknown(loc, *tokenstart);
			i = end;
			break;
	}
}

char lexer::adv(string::const_iterator &i, string::const_iterator end) {
	char c = 0;
	if (i != end) {
		c = *i;
		++pos.col;
		loc.end = pos;
		++i;
	}
	return c;
}

void lexer::ret(string::const_iterator &i, string::const_iterator end) {
	--pos.col;
	loc.end = pos;
	--i;
}
