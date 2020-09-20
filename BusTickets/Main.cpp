#include "TicketsSolver.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>


int main() {

	std::cout << "INPUT <- PRINT LENGTH AND GOAL YOU NEED : ";
	unsigned length = 0; int _goal = 0;
	std::cin >> length >> _goal;
#ifdef _TICKETS_SOLVER_STATIC_ARRAY_SIZE_
	if (length > _TICKETS_SOLVER_STATIC_ARRAY_SIZE_) {
		std::cout << "\nERROR : length can\'t be greater than " << _TICKETS_SOLVER_STATIC_ARRAY_SIZE_ << std::endl;
		return 1;
	}
#endif
	std::cout << "\nPRINT " << length << " RATIONAL NUMBERS TO EVALUATE : ";
	std::unique_ptr<unsigned[]> arr = std::make_unique<unsigned[]>(length);
	for (unsigned i = 0; i < length; i++) { std::cin >> arr[i]; }
	try {
		TicketsSolver ts(length, _goal, arr.get());

		std::cout << "\nOUTPUT ->\n\n";
				auto begin_time = std::chrono::steady_clock::now();
		//выводим все решения в поток
		unsigned count = ts.all_solutions(std::cout);
				auto end_time = std::chrono::steady_clock::now();
		std::cout << "\nCOUNT OF SOLUTIONS IS : " << count 
<< "  TIME : " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << " ms\n";

	} catch (std::exception e) {
		std::cout << "\nEXCEPTION { " << e.what() << " }\n";
		return -1;
	}
	
	return 0;
}


