#include "TicketsSolver.h"
#include <stdlib.h>


Rational* _init_data_Rational(const unsigned* data, unsigned size);
struct OpersConfig _init_OpersConfig(unsigned size);

int init_TicketsSolver(TicketsSolver* ts, unsigned size, int goal, const unsigned* data)
{
	if (size < 2 || goal < 0) return -1;
	
	ts->ticket = (struct Ticket)
	{
		(Rational) {goal, 1},
		_init_data_Rational(data, size),
		static_StrConverter_init_str_data(data, size),
		size
	};
	ts->opers_config = _init_OpersConfig(size);
	if (!(ts->ticket.data && ts->ticket.str_data && ts->opers_config.opers)) return 1;
	
	ts->permutator = (struct Permutator){ ts->opers_config, NORMAL_EVALUATION };
	if (!init_Evaluator(&ts->evaluator, size, ts->ticket.data, ts->opers_config)) return 1;
	if (!init_Str_Converter(&ts->str_converter, size, ts->ticket.str_data, ts->opers_config)) return 1;

	return 0;
}




unsigned count_of_solutions(TicketsSolver ts) {
	unsigned count = 0;

	TS_FOR_ALL_SOLUTIONS(ts, count++;)

	return count;
}


unsigned all_solutions(TicketsSolver  ts, FILE* out) {
	unsigned count = 0;
	void printer(TicketsSolver ts, FILE * out);
	TS_FOR_ALL_SOLUTIONS(ts, count++; printer(ts, out);)
	return count;
}

//....................

void printer(TicketsSolver ts, FILE* out) {
	char* buffer = StrConverter_normal_convert(ts.str_converter);
	if (buffer) {
		printf(" % d = %s\n", ts.ticket.goal.numer, buffer);
		free(buffer); buffer = NULL;
	}
	else {
		printf("ALLOCATE ERROR IN CONVERTER! TERMINATE.");
		exit(5);
	}
}

//.............


void destroy_TicketsSolver(TicketsSolver* ts)
{
	free(ts->opers_config.opers);
	free(ts->ticket.data);
	free(ts->ticket.str_data);
	destroy_Evaluator(ts->evaluator.list);
	
	ts->ticket.data = ts->evaluator.data = NULL;
	ts->ticket.str_data = ts->str_converter.str_data = NULL;
	ts->ticket.size = ts->permutator.opers_config.opers_size = ts->evaluator.list_size = ts->opers_config.opers_size = ts->str_converter.list_size = 0;
	ts->opers_config.opers = ts->evaluator.opers_config.opers = ts->permutator.opers_config.opers = ts->permutator.opers_config.opers = ts->str_converter.opers_config.opers = NULL;
}


////////////////////////////////////////////////////////////////////////////////////


Rational* _init_data_Rational(const unsigned* data, unsigned size) {

	Rational* const arr = malloc(size * sizeof(Rational));
	Rational* i = arr;
	const unsigned* _end = data + size;
	for (; data != _end; ++i, ++data) *i = (Rational){ *data, 1 };
	return arr;
}

struct OpersConfig _init_OpersConfig(unsigned size) {
	return (struct OpersConfig) { malloc((size - 1) * sizeof(struct token)), size - 1 };
}
