#include "TicketsSolver.h"

#include <numeric>
#include <iostream>

#define length 6

int main() {
	int arr[length] = { 9, 9, 9, 9, 9, 9 };
	TicketsSolver ts(length, 0, arr);
	ts.all_solutions(std::cout);
	return 0;
}