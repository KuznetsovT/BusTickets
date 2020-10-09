


//��������� ������������ �����

struct Rational {
	int numer;
	unsigned denumer;
};

//�������������� ���������� � ���������� Rational
struct Rational init__Rational(int numer, unsigned denumer);

//������ ������������ �����
struct Rational Make_Rational(int numer, unsigned denumer);

//��������� �����
void normalize(struct Rational* r);
void reverse(struct Rational* r);

///////////////////////////////////////////////////////////////////
//���������//

struct Rational unary_minus(struct Rational r);
struct Rational reversed(struct Rational r);


struct Rational plus(struct Rational a, struct Rational b);
struct Rational multiple(struct Rational a, struct Rational b);
struct Rational minus(struct Rational a, struct Rational b);
struct Rational divide(struct Rational a, struct Rational b);


