#include <stdbool.h>


//структура рациональных чисел

typedef struct Rational {
	int numer;
	unsigned denumer;
} Rational;

const static Rational INF = { 1, 0u };
const static Rational NaN = { 0, 0u };
const static Rational NILL = { 0, 1 };

//инициализирует переменные и возвращает Rational
Rational init_Rational(int numer, unsigned denumer);
//для целого числа
Rational init_Rational_by_int(int number);

//создаёт несократимую дробь
Rational Make_Rational(int numer, unsigned denumer);

//сокращаем дробь
void Rational_normalize(Rational* r);
void Rational_reverse(Rational* r);

///////////////////////////////////////////////////////////////////
//ОПЕРАТОРЫ//

Rational Rational_unary_minus(Rational r);
Rational Rational_reversed(Rational r);
Rational Rational_unsigned_reversed(Rational r);

Rational Rational_plus(Rational a, Rational b);
Rational Rational_multiple(Rational a, Rational b);
Rational Rational_minus(Rational a, Rational b);
Rational Rational_minus_plus(Rational a, Rational b);
Rational Rational_divide(Rational a, Rational b);
Rational Rational_unsigned_divide(Rational a, Rational b);

//....................................................

bool Rational_equal(Rational a, Rational b);

//returns true if a<b
bool Rational_less(Rational a, Rational b);

int Rational_compare(Rational a, Rational b);


//.....................................................