#include "TicketsSolver.h"

#include <iostream>
#include <fstream>

#define length 8
#define _goal 100

int main() {
	unsigned arr[length] = { 8, 8, 8, 8, 8, 8, 8, 8 };
	std::ofstream of("output.txt");
	TicketsSolver ts(length, _goal, arr);
		//this code
	std::cout << ts.all_solutions(of, TicketsSolver::NORMAL_NOTATION) << std::endl;
		//and this code do equal, but first one should be a bit faster
	if (ts.find_first_solution()) {
		unsigned count = 0;
		do {
			std::cout << " " << ts.goal << " = " << ts.to_str() << std::endl;
			count++;
		} while (ts.next_solution());
		std::cout << std::endl << "COUNT : " << count << std::endl;
	}
	return 0;
}


