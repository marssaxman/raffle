// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef LTS_H
#define LTS_H

namespace lts {
enum class atom { param, null, error, echo };
enum class leaf { number, string, symbol };
enum class branch { apply, lambda, match, join };
struct delegate {
	virtual void lts_atom(location, atom) = 0;
	virtual void lts_leaf(location, leaf, std::string) = 0;
	virtual void lts_branch(location, branch) = 0;
};
} // namespace lts

#endif //LTS_H
