#include <stdio.h>
#include "Rational.h"

int main() {
	struct Rational a, b, c;
	a = init_Rational_by_int(2);
	b = init_Rational(2, 4);
	b = Make_Rational(b.numer, b.denumer);

	struct Rational e = minus(b, a), q = reversed(unary_minus(b));
	return 0;
}