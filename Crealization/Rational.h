


//структура рациональных чисел

struct Rational {
	int numer;
	unsigned denumer;
};

const static struct Rational INF = { 1, 0u };
const static struct Rational NaN = { 0, 0u };
const static struct Rational NILL = { 0, 1 };

//инициализирует переменные и возвращает Rational
struct Rational init_Rational(int numer, unsigned denumer);
//для целого числа
struct Rational init_Rational_by_int(int number);

//создаёт несократимую дробь
struct Rational Make_Rational(int numer, unsigned denumer);

//сокращаем дробь
void normalize(struct Rational* r);
void reverse(struct Rational* r);

///////////////////////////////////////////////////////////////////
//ОПЕРАТОРЫ//

struct Rational unary_minus(struct Rational r);
struct Rational reversed(struct Rational r);
struct Rational unsigned_reversed(struct Rational r);

struct Rational plus(struct Rational a, struct Rational b);
struct Rational multiple(struct Rational a, struct Rational b);
struct Rational minus(struct Rational a, struct Rational b);
struct Rational minus_plus(struct Rational a, struct Rational b);
struct Rational divide(struct Rational a, struct Rational b);
struct Rational unsigned_divide(struct Rational a, struct Rational b);

//....................................................

int equal(struct Rational a, struct Rational b);
int less(struct Rational a, struct Rational b);

int compare(struct Rational a, struct Rational b);