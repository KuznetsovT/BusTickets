#pragma once

#include <ostream>


/*
Класс, описывающий рациональные числа.
Используем данный класс чтобы убрать ошибку округления в представлении double
*/

struct Rational {

	int numer = 0;
	unsigned denumer = 1;


	//конструктор не проверяет дробь на сокращаемость - используйте make_rational
	constexpr Rational(int numer = 0, unsigned denumer = 1) noexcept;

	//создаёт несократимую дробь
	static Rational make_Rational(int numer = 0, unsigned denumer = 1) noexcept;


	//функция вывода в поток
	friend std::ostream& operator<<(std::ostream& out, const Rational& r) noexcept;

	friend constexpr Rational operator-(const Rational& r) noexcept;

	friend Rational operator+(const Rational& l, const Rational& r) noexcept;
	friend Rational operator*(const Rational& l, const Rational& r) noexcept;
	friend Rational operator-(const Rational& l, const Rational& r) noexcept;
	friend Rational operator/(const Rational& l, const Rational& r) noexcept;


	friend bool operator==(const Rational& l, const Rational& r) noexcept;
	friend bool operator<(const Rational& l, const Rational& r) noexcept;
	friend bool operator>(const Rational& l, const Rational& r) noexcept;


	constexpr operator double() const;
	constexpr operator int()  const;
	constexpr Rational reversed() const noexcept;

	const static Rational INF;
	const static Rational NaN;

	bool is_negative() const noexcept;
	bool IS_INF() const noexcept;
	bool IS_NAN() const noexcept;
	bool IS_NULL() const noexcept;
	bool IS_NUMBER() const noexcept;

	void normalize() noexcept;
};


std::ostream& operator<<(std::ostream& out, const Rational& r) noexcept;

constexpr Rational operator-(const Rational& r) noexcept;

Rational operator+(const Rational& l, const Rational& r) noexcept;
Rational operator*(const Rational& l, const Rational& r) noexcept;
Rational operator-(const Rational& l, const Rational& r) noexcept;
Rational operator/(const Rational& l, const Rational& r) noexcept;


bool operator==(const Rational& l, const Rational& r) noexcept;
bool operator<(const Rational& l, const Rational& r) noexcept;
bool operator>(const Rational& l, const Rational& r) noexcept;
