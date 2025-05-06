#include <iostream>
#include "comp_club.h"

int main(int argc, char* argv[]) {

	Comp_Club club(argv[1]);
	club.parse_file();

	return 0;
}

