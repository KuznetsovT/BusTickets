#include "TicketsSolver.h"

#include <iostream>
#include <fstream>
#include <memory>


int main() {

	std::cout << "INPUT <- PRINT LENGTH AND GOAL YOU NEED : ";
	unsigned length = 0; int _goal = 0;
	std::cin >> length >> _goal;
	std::cout << "\nPRINT " << length << " RATIONAL NUMBERS TO EVALUATE : ";
	std::unique_ptr<unsigned[]> arr = std::make_unique<unsigned[]>(length);
	for (unsigned i = 0; i < length; i++) { std::cin >> arr[i]; }
	try {
		TicketsSolver ts(length, _goal, arr.get());

		std::cout << "\nOUTPUT ->\n\n";
		//выводим все решения в поток
		unsigned count = ts.all_solutions(std::cout);
		std::cout << "\nCOUNT OF SOLUTIONS IS : " << count << std::endl;

	} catch (std::exception e) {
		std::cout << "\nEXCEPTION { " << e.what() << " }\n";
		return -1;
	}
	
	return 0;
}


