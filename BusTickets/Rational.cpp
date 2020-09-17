#include "Rational.h"
#include <numeric>
/*
���� � ����������� ������ ������������ �����.

���������� std::gcd � std::lcm �� ����� <numeric>

!!!!!!!!!!!!!!�� �������� ��� ��������� �++17!!!!!!!!!!!!!!!!!!!!!!!!
*/

const Rational Rational::INF = Rational(1,0);

//������������//

//�������� ����� �� ������������� �� ����������!
//����������� make_Rational ��� �������� ������������ �����
Rational::Rational(int numer, unsigned denumer) noexcept
	: numer(numer), denumer(denumer) {}


//����������� ������ ������� ��� �������� ������������ �����
Rational Rational::make_Rational(int numer, unsigned denumer) noexcept
{
	unsigned gcd = std::gcd(numer, denumer);
	return Rational(numer / gcd, denumer / gcd);
}


//�������� ������ Rational
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

//return -(r)
inline Rational operator-(const Rational& r) noexcept
{
	return Rational(-r.numer, r.denumer);
}

//return l + r
Rational operator+(const Rational& l, const Rational& r) noexcept
{
	return Rational::make_Rational(l.numer*r.denumer+r.numer*l.denumer, r.denumer*l.denumer);
}

//return l * r
Rational operator*(const Rational& l, const Rational& r) noexcept
{
	return Rational::make_Rational(l.numer*r.numer, l.denumer*r.denumer);
}

//return l - r
Rational operator-(const Rational& l, const Rational& r) noexcept
{
	return l + (-r);
}

//return l / r
Rational operator/(const Rational& l, const Rational& r)
{
	return l * r.reversed();
}



//return double(r)
inline Rational::operator double() const
{
	return double(numer) / double(denumer);
}

//return int(r)
inline Rational::operator int() const
{
	return int(operator double());
}

inline Rational Rational::reversed() const
{
	return Rational(denumer, numer);
}

bool Rational::is_negative() const noexcept
{
	return numer < 0;
}

bool Rational::IS_INF() const noexcept
{
	return 0 == denumer;
}

bool Rational::IS_NAN() const noexcept
{
	return 0 == numer && 0 == denumer;
}

bool Rational::IS_NULL() const noexcept
{
	return 0 == numer && 0 != denumer;
}

inline bool Rational::IS_NUMBER() const noexcept
{
	return  0 != denumer;
}


//divide numerator and denumerator by gcd
inline void Rational::normalize() noexcept
{
	unsigned gcd = std::gcd(numer, denumer);
	numer /= gcd; denumer /= gcd;
}



bool operator==(const Rational& l, const Rational& r) noexcept
{
	return l.numer * r.denumer == l.denumer* r.numer;
}

bool operator<(const Rational& l, const Rational& r) noexcept
{
	return l.numer*r.denumer <  l.denumer*r.numer;
}

bool operator>(const Rational& l, const Rational& r) noexcept
{
	return l.numer*r.denumer > r.numer*l.denumer;
}
