


//структура рациональных чисел

struct Rational {
	int numer;
	unsigned denumer;
};

//инициализирует переменные и возвращает Rational
struct Rational init__Rational(int numer, unsigned denumer);

//создаёт несократимую дробь
struct Rational Make_Rational(int numer, unsigned denumer);

//сокращаем дробь
void normalize(struct Rational* r);
void reverse(struct Rational* r);

///////////////////////////////////////////////////////////////////
//ОПЕРАТОРЫ//

struct Rational unary_minus(struct Rational r);
struct Rational reversed(struct Rational r);


struct Rational plus(struct Rational a, struct Rational b);
struct Rational multiple(struct Rational a, struct Rational b);
struct Rational minus(struct Rational a, struct Rational b);
struct Rational divide(struct Rational a, struct Rational b);


