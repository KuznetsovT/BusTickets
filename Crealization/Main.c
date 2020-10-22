#define _CRT_SECURE_NO_WARNINGS

#include "TIME_MEASURE.h"


#include "TicketsSolver.h"
#include <stdlib.h>



int main() {
	
	int length = 2;
	int goal = 0;
	printf("Print length and goal: ");
	if (scanf("%d%d", &length, &goal) != 2) {
		printf("No arguments. Terminate\n");
		return 1;
	}
	if (length < 2) {
		printf("Length must be greater than 1. Terminate\n");
		return -1;
	}
	if (goal < 0) {
		printf("For evaluation optimization goal should be possitive! Terminate");
		return -1;
	}
	unsigned* uns_data = malloc(length * sizeof(unsigned));
	if (!(uns_data)) {
		printf("ALLOCATE ERROR. TERMINATE.\n");
		return 2;
	}
	printf("Print %u unsigned numbers to evaluate: ", length);
	
	for (unsigned i = 0; i != length; ++i) {
		if (!scanf("%u", &uns_data[i])) {
			printf("\nINPUT ERROR. TERMINATE\n");
			return 3;
		}
	}
	putchar('\n');
	TicketsSolver ts;
	if (init_TicketsSolver(&ts, length, goal, uns_data)) {
		printf("TicketsSolver initialization error. Terminate\n");
		return 4;
	}
	//ts.permutator._WORKING_OPERATORS = ONLY_SUM;
	unsigned count = 0;
	TIME_MEASURE_BEGIN(measure)
		count = all_solutions(ts, stdout);
	TIME_MEASURE_END(measure)
		printf("\nCOUNT OF SOLUTIONS : %u    TIME : %.1lf ms\n", count, measure);
	return 0;
}