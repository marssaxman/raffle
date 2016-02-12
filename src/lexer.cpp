// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "lexer.h"

static bool is_combining_punct(char c)
{
	switch (c) {
		case '!':
		case '$':
		case '%':
		case '&':
		case '*':
		case '+':
		case '-':
		case '.':
		case '/':
		case ':':
		case '<':
		case '=':
		case '>':
		case '?':
		case '@':
		case '\\':
		case '^':
		case '`':
		case '|':
		case '~':
			return true;
		default:
			return false;
	}
}

void lexer::read_line(const std::string &input)
{
	pos.col = 0;
	auto i = input.begin();
	while (i != input.end() && isblank(*i)) {
		++i;
	}
	unsigned indent = i - input.begin();
	out.indent(pos, indent);
	pos.col = indent;
	for (pos.col = indent; i != input.end(); pos.col = i - input.begin()) {
		auto tokenstart = i;
		switch (char c = *i++) {
			case '#':
				// Comment extends to end of line
				i = input.end();
				break;

			case '(':
			case '[':
			case '{':
				out.opener(pos, c);
				break;

			case ')':
			case ']':
			case '}':
				out.closer(pos, c);
				break;

			case '\"':
			case '\'':
				do {
					if (i == input.end()) {
						err.nonterminated(pos);
						break;
					}
				} while (*i++ != c);
				out.string(pos, std::string(tokenstart, i));
				break;

			case ' ':
			case '\t':
				while (i != input.end() && isblank(*i)) {
					++i;
				}
				out.spacer(pos);
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
				while (i != input.end() && isalnum(*i)) {
					++i;
				}
				out.number(pos, std::string(tokenstart, i));
				break;

			case '_':
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
				while (i != input.end() && isalnum(*i) || '_' == *i) {
					++i;
				}
				out.symbol(pos, std::string(tokenstart, i));
				break;

			case ',':
			case ';':
				out.rubric(pos, std::string(1, c));
				break;

			case '!':
			case '$':
			case '%':
			case '&':
			case '*':
			case '+':
			case '-':
			case '.':
			case '/':
			case ':':
			case '<':
			case '=':
			case '>':
			case '?':
			case '@':
			case '\\':
			case '^':
			case '`':
			case '|':
			case '~':
				while (i != input.end() && is_combining_punct(*i)) {
					++i;
				}
				out.rubric(pos, std::string(tokenstart, i));
				break;

			default:
				err.unknown(pos, c);
				pos.col = input.size();
				break;
		}
	}
	out.newline(pos);
}

void lexer::read_file(std::istream &in)
{
	pos.row = 1;
	for (std::string line; std::getline(in, line); ++pos.row) {
		read_line(line);
	}
}


