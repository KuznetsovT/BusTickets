#include "TicketsSolver.h"

#include <iostream>
#include <fstream>

#define length 3
#define _goal 3

int main() {
	unsigned arr[length] = { 0 };
	std::cout << "INPUT. PRINT 6 NATURAL NUMBERS: ";
	for (auto& i : arr) { std::cin >> i; }
	TicketsSolver ts(length, _goal, arr);

	//хочу чтобы выводились только тривиальные решения без деления.
	ts.permutator.WORKING_OPERATORS = TicketsSolver::Permutator::NO_DIVISION;

	//потом я хочу вывести решения в обратной польской нотации

	std::cout << ts.all_solutions(std::cout, TicketsSolver::REVERSED_NOTATION);

	/*
	* но готовый метод - слишком легко, тем более что TicketsSolver  состоит из трёх классов:
	*	permutator, evaluator и str_converter, 
	*	у которых есть публичные методы позволяющие писать свои переборы, отличные от уже реализованных.
	* Сейчас я попробую это сделать.
	*/

	/*
	for (ts.permutator.reinit_signs();                                                         
		ts.permutator.are_signs_valid();                                                       
		ts.permutator.next_sign_configuration()) {

			if (ts.permutator.min_unique_pos()) do {

					if (ts.goal == ts.evaluator.evaluate()) { 
						std::cout << " " << ts.goal << " = " << ts.str_converter.convert(TicketsSolver::REVERSED_NOTATION) << std::endl;
					}                            
						
			} while (ts.permutator.next_operators_configuration());                                
						
	}
	*/


	/*
	Так же, если лень писать,
	можно воспользоваться методами которые ищут решение и сохраняют в себе конфигурацию операторов.
	Я мог сделать нечто такое:
	Но оно работает не совсем оптимально, так как в next_solution содержатся дополнительные проверки на дурака.
	*/

	/*
			if (ts.find_first_solution()) do {

				//здесь могла быть ваша реклама
					std::cout << ts.goal << " = " << ts.str_converter.convert(TicketsSolver::REVERSED_NOTATION) << std::endl;

			} while (ts.next_solution());

	*/
	
	//при отменной лени можно воспользоваться TS_FOR_ALL_SOLUTIONS(_ts_, _expr_)
	
	return 0;
}


