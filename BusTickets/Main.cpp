#include "TicketsSolver.h"

#include <iostream>

#define length 8

int main() {
	unsigned arr[length] = { 9, 9, 9, 9, 9, 9, 9, 9};
	TicketsSolver ts(length, 100, arr);
	ts.all_solutions(std::cout);
	return 0;
}


//ещ нужно сделать невозможным (-+)*