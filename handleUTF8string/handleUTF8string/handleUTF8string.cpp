// Example program https://www.cprogramming.com/tutorial/unicode.html
#include "stdafx.h"
#include <iostream>
#include <string>
#include <cstdint>
using namespace std;

#include "utf8.h"
const uint32_t MAXNUMBER_PER_LINE = 20;

int main(int argc, char** argv)
{
	/*if (argc != 3)
	{
		cerr << "argument is not matched" << "\n";
		return EXIT_FAILURE;
	}

	cout << "You have entered " << argc << " arguments:" << "\n";
	for (int i = 0; i < argc; ++i)
		cout << argv[i] << "\n";*/

	std::string text(u8"2000‰~ˆÈã‚Ì‚²—˜—p‚Å1000‰~ˆø‚«ƒN[ƒ|ƒ“");
	utf8 handle_utf8_Object;
	handle_utf8_Object.init_from_string(text.c_str());
	handle_utf8_Object.breakline(MAXNUMBER_PER_LINE);
	getchar();
	return 0;
}
