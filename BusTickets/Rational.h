#pragma once

#include <ostream>
#include <numeric>

/*
Класс, описывающий рациональные числа.
Используем данный класс чтобы убрать ошибку округления в представлении double
*/

struct Rational {

	int numer = 0;
	unsigned denumer = 1;


	//конструктор не проверяет дробь на сокращаемость - используйте make_rational
	inline constexpr Rational(int numer = 0, unsigned denumer = 1) noexcept;

	//создаёт несократимую дробь
	inline static Rational make_Rational(int numer = 0, unsigned denumer = 1) noexcept;


	//функция вывода в поток
	friend inline std::ostream& operator<<(std::ostream& out, const Rational& r) noexcept;

	friend inline Rational operator-(const Rational& r) noexcept;

	friend inline Rational operator+(const Rational& l, const Rational& r) noexcept;
	friend inline Rational operator*(const Rational& l, const Rational& r) noexcept;
	friend inline Rational operator-(const Rational& l, const Rational& r) noexcept;
	friend inline Rational operator/(const Rational& l, const Rational& r) noexcept;


	friend inline bool operator==(const Rational& l, const Rational& r) noexcept;
	friend inline bool operator<(const Rational& l, const Rational& r) noexcept;
	friend inline bool operator>(const Rational& l, const Rational& r) noexcept;


	inline operator double() const;
	inline operator int()  const;
	inline Rational reversed() const noexcept;

	static const Rational INF;
	static const Rational NaN;

	inline bool is_negative() const noexcept;
	inline bool IS_INF() const noexcept;
	inline bool IS_NAN() const noexcept;
	inline bool IS_NULL() const noexcept;
	inline bool IS_NUMBER() const noexcept;

	inline void normalize() noexcept;
};


inline std::ostream& operator<<(std::ostream& out, const Rational& r) noexcept;

inline Rational operator-(const Rational& r) noexcept;

inline Rational operator+(const Rational& l, const Rational& r) noexcept;
inline Rational operator*(const Rational& l, const Rational& r) noexcept;
inline Rational operator-(const Rational& l, const Rational& r) noexcept;
inline Rational operator/(const Rational& l, const Rational& r) noexcept;


inline bool operator==(const Rational& l, const Rational& r) noexcept;
inline bool operator<(const Rational& l, const Rational& r) noexcept;
inline bool operator>(const Rational& l, const Rational& r) noexcept;




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*
Реализация класса рациональных чисел.

Используем std::gcd и std::lcm из файла <numeric>

!!!!!!!!!!!!!!НЕ РАБОТАЕТ БЕЗ СТАНДАРТА С++17!!!!!!!!!!!!!!!!!!!!!!!!
*/


//КОНСТРУКТОРЫ//

//проверки дроби на сокращаемость не происходит!
//используйте make_Rational для создания несократимой дроби
inline constexpr Rational::Rational(int numer, unsigned denumer) noexcept
	: numer(numer), denumer(denumer) {}


//используйте данную функцию для создания несократимой дроби
inline Rational Rational::make_Rational(int numer, unsigned denumer) noexcept
{
	unsigned gcd = std::gcd(numer, denumer);
	return Rational(numer / (int)gcd, denumer / gcd);
}


//divide numerator and denumerator by gcd
inline void Rational::normalize() noexcept
{
	unsigned gcd = std::gcd(numer, denumer);
	numer /= (int)gcd; denumer /= gcd;
}


//оператор вывода Rational
inline std::ostream& operator<<(std::ostream& out, const Rational& r) noexcept
{
	if (r.denumer != 1)
		out << r.numer << "/" << r.denumer;
	else
		out << r.numer;
	return out;
}

//...............................................................................
//...............................................................................
//Операторы

//return -(r)
inline Rational operator-(const Rational& r) noexcept
{
	return Rational(-r.numer, r.denumer);
}

//return l + r
inline Rational operator+(const Rational& l, const Rational& r) noexcept
{
	return Rational::make_Rational(l.numer * r.denumer + r.numer * l.denumer, r.denumer * l.denumer);
}

//return l * r
inline Rational operator*(const Rational& l, const Rational& r) noexcept
{
	return Rational::make_Rational(l.numer * r.numer, l.denumer * r.denumer);
}

//return l - r
inline Rational operator-(const Rational& l, const Rational& r) noexcept
{
	return l + (-r);
}

//return l / r
inline Rational operator/(const Rational& l, const Rational& r) noexcept
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

inline Rational Rational::reversed() const noexcept
{
	return Rational(denumer, numer);
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//Булевы функции с проверкой условий или сравнения

inline bool Rational::is_negative() const noexcept
{
	return numer < 0;
}

inline bool Rational::IS_INF() const noexcept
{
	return 0 == denumer;
}

inline bool Rational::IS_NAN() const noexcept
{
	return 0 == numer && 0u == denumer;
}

inline bool Rational::IS_NULL() const noexcept
{
	return 0 == numer && 0u != denumer;
}

inline bool Rational::IS_NUMBER() const noexcept
{
	return  0u != denumer;
}

//???????????????????????????????????????????????????????????????????????????????

inline bool operator==(const Rational& l, const Rational& r) noexcept
{
	return l.numer * (int)r.denumer == (int)l.denumer * r.numer;
}

inline bool operator<(const Rational& l, const Rational& r) noexcept
{
	return l.numer * (int)r.denumer < (int)l.denumer* r.numer;
}

inline bool operator>(const Rational& l, const Rational& r) noexcept
{
	return l.numer * (int)r.denumer > (int)l.denumer * r.numer;
}
