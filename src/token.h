// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef TOKEN_H
#define TOKEN_H

#include "location.h"
#include <string>

namespace token {

struct delegate {
	virtual void token_eof(location) = 0;
	virtual void token_number(location, std::string) = 0;
	virtual void token_symbol(location, std::string) = 0;
	virtual void token_string(location, std::string) = 0;
	virtual void token_underscore(location) = 0;
	virtual void token_l_paren(location) = 0;
	virtual void token_r_paren(location) = 0;
	virtual void token_l_bracket(location) = 0;
	virtual void token_r_bracket(location) = 0;
	virtual void token_l_brace(location) = 0;
	virtual void token_r_brace(location) = 0;
	virtual void token_comma(location) = 0;
	virtual void token_semicolon(location) = 0;
	virtual void token_colon(location) = 0;
	virtual void token_dot(location) = 0;
	virtual void token_dot_dot(location) = 0;
	virtual void token_plus(location) = 0;
	virtual void token_hyphen(location) = 0;
	virtual void token_star(location) = 0;
	virtual void token_slash(location) = 0;
	virtual void token_percent(location) = 0;
	virtual void token_ampersand(location) = 0;
	virtual void token_pipe(location) = 0;
	virtual void token_caret(location) = 0;
	virtual void token_bang(location) = 0;
	virtual void token_equal(location) = 0;
	virtual void token_bang_equal(location) = 0;
	virtual void token_l_angle(location) = 0;
	virtual void token_r_angle(location) = 0;
	virtual void token_l_bangle(location) = 0;
	virtual void token_r_bangle(location) = 0;
	virtual void token_l_guillemet(location) = 0;
	virtual void token_r_guillemet(location) = 0;
	virtual void token_l_arrow(location) = 0;
	virtual void token_r_arrow(location) = 0;
};

} // namespace token

#endif //TOKEN_H
