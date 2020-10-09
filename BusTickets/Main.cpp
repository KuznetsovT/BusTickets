#include "TicketsSolver.h"

#include <memory>
#include <chrono>

#include <cstdio>   //при помощи printf будем выводить критичные места программы
#include <iostream> //осуществляем ввод, и вывод некритичных данных

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
		auto begin_time = std::chrono::steady_clock::now();

		//выводим все решения в поток
		unsigned count = ts.all_solutions(); //неявно передаём сишный stdout

		auto end_time = std::chrono::steady_clock::now();
		std::cout << "\nCOUNT OF SOLUTIONS IS : " << count << "  TIME : " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() << " ms\n";

	} catch (std::exception e) {
		std::cout << "\nEXCEPTION { " << e.what() << " }\n";
		return -1;
	}
	
	return 0;
}


