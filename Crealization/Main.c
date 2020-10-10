
#include "Rational.h"
#include "TIME_MEASURE.h"



int main() {
	
	TIME_MEASURE
	(
		for (unsigned i = 0; i < 100000; ++i) for (unsigned j = 0; j < 10000; ++j) { int h = i * j; },
		"HARD"
	)
	return 0;
}