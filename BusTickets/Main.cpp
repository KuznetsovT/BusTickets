#include "TicketsSolver.h"

#include <iostream>

#define length 3

int main() {
	unsigned arr[length] = { 1,2,3 };
	TicketsSolver ts(length, 4, arr);
	ts.all_solutions(std::cout);
	return 0;
}


