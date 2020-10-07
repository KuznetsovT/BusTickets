#include "Rational.h"

/*
Файл с реализацией класса рациональных чисел.

Используем std::gcd и std::lcm из файла <numeric>

!!!!!!!!!!!!!!НЕ РАБОТАЕТ БЕЗ СТАНДАРТА С++17!!!!!!!!!!!!!!!!!!!!!!!!
*/

const Rational Rational::INF = Rational(1, 0);
const Rational Rational::NaN = Rational(0, 0);




//оператор вывода Rational
std::ostream& operator<<(std::ostream& out, const Rational& r) noexcept
{
	if (r.denumer != 1) {
		out << r.numer << "/" << r.denumer;
	}
	else {
		out << r.numer;
	}
	return out;
}

//Большинство функций определяем как inline
