
#include "Rational.h"
#include "TIME_MEASURE.h"

#include "Permutator.h"
#include "Evaluator.h"
#include "StrConverter.h"
#include <stdlib.h>

#define length 7
#define _goal 100


void to_do(struct Permutator per, struct Evaluator eval, struct StrConverter strconv, Rational goal) {


	


	for (Permutator_reinit_signs(per.opers_config);
		Permutator_are_signs_valid(per); Permutator_next_sign_configuration(per))
	{
		Permutator_reinit_pos(per.opers_config);
		if (Permutator_min_unique_pos(per.opers_config)) do {

			if (Rational_equal(goal, Evaluator_evaluate(eval)))
			{
				char* buffer = StrConverter_reversed_convert(strconv);
				printf(" % d/%u = %s\n", goal.numer, goal.denumer, buffer);
				free(buffer);
			}

		} while (Permuator_next_operators_configuration(per.opers_config));

	}
}

int main() {
	
	Rational goal = { _goal, 1 };

	unsigned* uns_data = malloc(length * sizeof(unsigned));
	Rational* data = malloc(length * sizeof(Rational));
	for (unsigned i = 0; i != length; ++i) data[i] = (Rational){ uns_data[i] = i, 1 };

	struct OpersConfig conf = { malloc((length - 1) * sizeof(struct token)), (length - 1) };
	struct Permutator per = { conf, NORMAL_EVALUATION };
	struct Evaluator eval; init_Evaluator(&eval, length, data, conf);
	struct StrConverter strconv;
	init_Str_Converter(&strconv, length, static_StrConverter_init_str_data(uns_data, length), conf);

	TIME_MEASURE(to_do(per, eval, strconv, goal); , "TICKETS");
	return 0;
}