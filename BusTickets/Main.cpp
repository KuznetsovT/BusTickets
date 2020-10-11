#include "TicketsSolver.h"

#include <memory>
#include <chrono>

#include <cstdio>   //при помощи printf будем выводить критичные места программы
#include <iostream> //осуществляем ввод, и вывод некритичных данных

#include "TIME_MEASURE.h"

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
		
		TIME_MEASURE_NO_DESCRIPITON(
		//выводим все решения в поток
		unsigned count = ts.all_solutions(); //неявно передаём сишный stdout
		std::cout << "\nCOUNT OF SOLUTIONS IS : " << count;
		);

	} catch (std::exception e) {
		std::cout << "\nEXCEPTION { " << e.what() << " }\n";
		return -1;
	}
	
	return 0;
}


