#include "TicketsSolver.h"

#include <iostream>
#include <fstream>
#include <chrono>

#define length 3
#define _goal 100

// в данной реализации нам видны приватные поля TS для большей эффективности 

int main() {
	unsigned arr[length] = { 0, 0, 0 };

	TicketsSolver ts(length, _goal, arr);


	auto begin_time = std::chrono::steady_clock::now();

	while (ts.data[length - 1].numer < 10) {


		ts.permutator.WORKING_OPERATORS = ts.permutator.NO_DIVISION;
		if (!ts.find_first_solution()) {
			ts.permutator.WORKING_OPERATORS = ts.permutator.NORMAL_EVALUATION;
			unsigned normal_count = ts.count_of_solutions();
			if (normal_count == 1) {
				for (auto i = ts.data, _end = ts.data + ts.size; i != _end; i++) {
					std::cout << i->numer << " ";
				}
				std::cout << " : " << normal_count << "  " << ts.first_solution() << std::endl;
			}
		}
		
		ts.data[0].numer++;
		for (auto i = ts.data, j = ts.data + 1; i != ts.data + length - 1; i++, j++) {
			if (i->numer == 10) {
				i->numer = 0; j->numer++;
			}
			else break;
		}
	}
	auto end_time = std::chrono::steady_clock::now();

	std::cout << std::endl << " TIME : " << 
	std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << " ms\n";
	return 0;
}


