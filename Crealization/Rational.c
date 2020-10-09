#include "Rational.h"
#include <math.h>

//нахождение НОК
unsigned gcd(int a, int b);


//инициализируем дробь
struct Rational init_Rational(int numer, unsigned denumer)
{
	return (struct Rational) { numer, denumer };	
}


struct Rational init_Rational_by_int(int number)
{
	return (struct Rational) { number, 1u };
}

//создаёт несократимую дробь
struct Rational Make_Rational(int _numer, unsigned _denumer) {
	unsigned _gcd = gcd(_numer, _denumer);
	return (struct Rational){ _numer / (int)_gcd, _denumer / _gcd };
}

//сокращаем дробь
void normalize(struct Rational* r) {
	unsigned _gcd = gcd(r->numer, r->denumer);
	r->numer /= (int)_gcd; r->denumer /= _gcd;
}

//переворот дроби
void reverse(struct Rational* r) {
	*r = reversed(*r);
}

/////////////////////////////////////////////////////////////////

struct Rational unary_minus(struct Rational r)
{
	return (struct Rational){ -r.numer, r.denumer };
}


struct Rational reversed(struct Rational r)
{
	return (r.numer >= 0) ? (struct Rational) { r.denumer, r.numer } : (struct Rational) { -(int)r.denumer, -r.numer };
}

//WARNING r.numer >= 0 !
struct Rational unsigned_reversed(struct Rational r)
{
	return (struct Rational) { r.denumer, r.numer };
}

struct Rational plus(struct Rational a, struct Rational b)
{
	return Make_Rational(a.numer * b.denumer + a.denumer * b.numer, a.denumer * b.denumer);
}

struct Rational multiple(struct Rational a, struct Rational b)
{
	return Make_Rational(a.numer*b.numer, a.denumer*b.denumer);
}

struct Rational minus(struct Rational a, struct Rational b)
{
	return plus( a, unary_minus(b) );
}

struct Rational minus_plus(struct Rational a, struct Rational b)
{
	return plus( unary_minus(a), b );
}

struct Rational divide(struct Rational a, struct Rational b)
{
	return multiple( a, reversed(b) );
}

struct Rational unsigned_divide(struct Rational a, struct Rational b)
{
	return multiple( a, unsigned_reversed(b) );
}

//.............................................................

//if a < b  -> (compare < 0)
int compare(struct Rational a,struct Rational b)
{
	return a.numer*b.denumer - b.numer*a.denumer;
}

int equal(struct Rational a, struct Rational b)
{
	return a.numer*b.denumer == b.numer*a.denumer;
}

int less(struct Rational a, struct Rational b) {
	return a.numer * b.denumer < b.numer* a.denumer;
}

///////////////////////////////////////////////////////////////

//нахождение НОК
unsigned gcd(int a, int b)
{
	a = abs(a);
	int for_swap;
	while (a) {
		for_swap = b % a;
		b = a;
		a = for_swap;
	}
	return b;
}