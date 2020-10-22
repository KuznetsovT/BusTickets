#include "Rational.h"
#include <math.h>

//нахождение НОК
unsigned gcd(int a, int b);


//инициализируем дробь
Rational init_Rational(int numer, unsigned denumer)
{
	return (struct Rational) { numer, denumer };	
}


Rational init_Rational_by_int(int number)
{
	return (struct Rational) { number, 1u };
}

//создаёт несократимую дробь
Rational Make_Rational(int _numer, unsigned _denumer) {
	unsigned _gcd = gcd(_numer, _denumer);
	return (struct Rational){ _numer / (int)_gcd, _denumer / _gcd };
}

//сокращаем дробь
void Rational_normalize(struct Rational* r) {
	unsigned _gcd = gcd(r->numer, r->denumer);
	r->numer /= (int)_gcd; r->denumer /= _gcd;
}

//переворот дроби
void Rational_reverse(struct Rational* r) {
	*r = Rational_reversed(*r);
}

/////////////////////////////////////////////////////////////////

Rational Rational_unary_minus(Rational r)
{
	return (struct Rational){ -r.numer, r.denumer };
}


Rational Rational_reversed(struct Rational r)
{
	return (r.numer >= 0) ? (struct Rational) { r.denumer, r.numer } : (struct Rational) { -(int)r.denumer, -r.numer };
}

//WARNING r.numer >= 0 !
struct Rational Rational_unsigned_reversed(struct Rational r)
{
	return (struct Rational) { r.denumer, r.numer };
}

struct Rational Rational_plus(struct Rational a, struct Rational b)
{
	return Make_Rational(a.numer * b.denumer + a.denumer * b.numer, a.denumer * b.denumer);
}

struct Rational Rational_multiple(struct Rational a, struct Rational b)
{
	return Make_Rational(a.numer*b.numer, a.denumer*b.denumer);
}

struct Rational Rational_minus(struct Rational a, struct Rational b)
{
	return Rational_plus( a, Rational_unary_minus(b) );
}

struct Rational Rational_minus_plus(struct Rational a, struct Rational b)
{
	return Rational_plus( Rational_unary_minus(a), b );
}

struct Rational Rational_divide(struct Rational a, struct Rational b)
{
	return Rational_multiple( a, Rational_reversed(b) );
}

struct Rational Rational_unsigned_divide(struct Rational a, struct Rational b)
{
	return Rational_multiple( a, Rational_unsigned_reversed(b) );
}

//.............................................................

//if a < b  -> (compare < 0)
int Rational_compare(struct Rational a,struct Rational b)
{
	return a.numer*b.denumer - b.numer*a.denumer;
}

bool Rational_equal(struct Rational a, struct Rational b)
{
	return a.numer*b.denumer == b.numer*a.denumer;
}

//returns true if a < b
bool Rational_less(struct Rational a, struct Rational b) {
	return a.numer * b.denumer < b.numer* a.denumer;
}

///////////////////////////////////////////////////////////////

//нахождение НОК
unsigned gcd(int a, int b)
{
	if (!a && !b) return 1u;
	int for_swap;
	while (a) {
		for_swap = b % a; b = a; a = for_swap;
	}
	return abs(b);
}