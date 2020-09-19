#include "TicketsSolver.h"

#include <iostream>
#include <fstream>

#define length 6
#define _goal 100

int main() {
	unsigned arr[length] = { 0, 0, 0, 0, 0, 0 };
	std::cout << "INPUT. PRINT 6 NATURAL NUMBERS: ";
	for (auto& i : arr) { std::cin >> i; }
	TicketsSolver ts(length, _goal, arr);

	//хочу чтобы выводились только тривиальные решения без деления.
	ts.permutator.OPERATORS_COUNT = TicketsSolver::Permutator::NO_DIVISION;

	//потом я хочу вывести решения в обратной польской нотации

	ts.all_solutions(std::cout, TicketsSolver::REVERSED_NOTATION);

	/*
	* но готовый метод - слишком легко, тем более что TicketsSolver  состоит из трёх классов:
	*	permutator, evaluator и str_converter, 
	*	у которых есть публичные методы позволяющие писать свои переборы, отличные от уже реализованных.
	* Сейчас я попробую это сделать.
	*/

	/*
			for (ts.permutator.reinit_signs();
				ts.permutator.are_signs_valid(); 
				ts.permutator.next_sign_configuration()) 
			{
				ts.permutator.reinit_pos();
				while (ts.permutator.is_doubled()) ts.permutator.next_operators_permutation();
				while (ts.permutator.are_poses_valid()) {
					if (ts.goal == ts.evaluator.evaluate()) {
						std::cout << ts.goal << " = " << ts.str_converter.convert(TicketsSolver::REVERSED_NOTATION) << std::endl;
					}
					do ts.permutator.next_operators_permutation(); while (ts.permutator.is_doubled());
				}
			}
	*/


	/*
	Так же, если лень писать,
	можно воспользоваться методами которые ищут решение и сохраняют в себе конфигурацию операторов.
	Я мог сделать нечто такое:
	*/

	/*
			if (ts.find_first_solution()) do {

				//здесь могла быть ваша реклама
					std::cout << ts.goal << " = " << ts.str_converter.convert(TicketsSolver::REVERSED_NOTATION) << std::endl;

			} while (ts.next_solution());

	*/
	
	
	return 0;
}


