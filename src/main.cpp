// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include <iostream>
#include <fstream>
#include "scanner.h"

class ifsource: public source<char>
{
	std::ifstream &in;
public:
	ifsource(std::ifstream &i): in(i) {}
	bool ready() override { return in.good(); }
	char produce() { return in.get(); }
};

int main(int argc, const char *argv[])
{
    if (argc < 1) {
        std::cerr << "no input files\n";
    } else for (int i = 1; i < argc; ++i) {
		std::ifstream file(argv[i]);
		ifsource floo(file);
		scanner flap;


		// now I need to attach this process to that source
    }
	return EXIT_SUCCESS;
}

