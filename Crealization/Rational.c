#include "Rational.h"
#include <math.h>

//���������� ���
unsigned gcd(int a, int b);


//�������������� �����
struct Rational init__Rational(int numer, unsigned denumer)
{
	return (struct Rational) { numer, denumer };	
}


//������ ������������ �����
struct Rational Make_Rational(int _numer, unsigned _denumer) {
	unsigned _gcd = gcd(_numer, _denumer);
	return (struct Rational){ _numer / (int)_gcd, _denumer / _gcd };
}

//��������� �����
void normalize(struct Rational* r) {
	unsigned _gcd = gcd(r->numer, r->denumer);
	r->numer /= (int)_gcd; r->denumer /= _gcd;
}

//��������� �����
void reverse(struct Rational* r) {
	
}


//���������� ���
unsigned gcd(int a, int b)
{
	int for_swap;
	while (a) {
		for_swap = b % a;
		b = a;
		a = for_swap;
	}
	return abs(b);
}