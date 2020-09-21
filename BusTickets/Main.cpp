#include "TicketsSolver.h"

#include <iostream>
#include <fstream>
#include <chrono>

#define length 6
#define _goal 100

int main() {
	unsigned arr[length] = { 0, 0, 0, 0, 0, 0 };

	TicketsSolver ts(length, _goal, arr);

	unsigned count = 0;

	auto begin_time = std::chrono::steady_clock::now();
	while (arr[length - 1] < 10) {

		if (ts.count_of_solutions() != 0) {
			count++;
		}
		arr[0]++;
		for (auto i = arr, j = arr + 1; i != arr + length - 1; i++, j++) {
			if (*i == 10) {
				*i = 0; (*j)++;
			}
			else break;
		}
		ts.set_new_data(arr);
	}
	auto end_time = std::chrono::steady_clock::now();

	std::cout << "COUNT : " << count << 
	" TIME : " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << " ms\n";
	return 0;
}


