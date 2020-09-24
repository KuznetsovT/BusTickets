#include "TicketsSolver.h"

#include <iostream>
#include <fstream>
#include <chrono>

#define length 6
#define _goal 100

// в данной реализации поля нам видны приватные поля TS для большей эффективности 

int main() {
	unsigned arr[length] = { 0, 0, 0, 0, 0, 0 };

	TicketsSolver ts(length, _goal, arr);

	unsigned count = 0;

	auto begin_time = std::chrono::steady_clock::now();
	while (ts.data[length - 1].numer < 10) {

		if (ts.find_first_solution() != 0) {
			count++;
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

	std::cout << "COUNT : " << count << 
	" TIME : " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << " ms\n";
	return 0;
}


