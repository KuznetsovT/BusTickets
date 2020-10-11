#include "Permutator.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

static const OPERATORS
	PLUS = 0u,    //id: 0
	MINUS = 1u,
	MINUS_PLUS = 2u,  //����������� ����: -(a) + b ���������� ~
	MULTIPLE = 3u,
	DIVIDE = 4u; //id: 4


//��� ������� �������� ������������ ��� 5 ������
const unsigned NORMAL_EVALUATION = OPERATORS_COUNT;
//���� ����� ������� ����������� ������� ��� ������� �������������� � OPERATORS_SIZE  = NO_DIVISION
const unsigned NO_DIVISION = 4u;
//���� ����� ������� ������ �� ������ � �������
const unsigned ONLY_SUM = 3u;

/*� ������� ���������� ���������� ����� ���������� ���������� ������ ���� ����� ��������� �������,
		����� ��� �� ��������� �������� ������ ����������� ������*/
static const unsigned diff_factor[OPERATORS_COUNT][OPERATORS_COUNT];

//.................................................................................
//.................................................................................

//����������
//������ ������������� � ��������������� ������� ����������


//����� ��� ����� ����������� ������������������ ����� �������
//WARNING: �� ���������� �������� ��� end <= opers_size
static void _Permutator_reinit_signs_part(struct token* opers, unsigned begin, unsigned end);



/*������������� ������� ���������� � ���� [ 1, 2, 3, 4, 5, 6...]
�������� ������������� ����������� ���������������� ��������
(������ ��� �������� ����������� �������� ����� begin).
��� ����� ����� ������������� ������������� ������������������
*/
//WARNING: �� ���������� �������� ��� end <= opers_size
//���������, ��� ��� �������� ����� ��������������� �� ������ min_value, ����� ������������������ �� ����� �����������
static void _Permutator_reinit_pos_part(struct token* opers, unsigned begin, unsigned end, unsigned min_value);
//���������� ������ _Permutator_reinit_part(p, begin, end, zero);
static void _Permutator_reinit_pos_part_with_zero(struct token *opers, const unsigned begin, const unsigned end);



//��������� ������� ���������� _Permutator_reinit_pos_part � �ר��� ������������(c� ���� ������������). 
//num - ����������� ������������� �������� �������(begin), ���� ����� ������ ���������� ������� ���� ��� ��������.
//�� ������� [begin, end) ������������� ����������� ��������������� ������������ ������� ����������
static void _Permutator_minimize_pos(struct token* begin, unsigned num, struct token* end);

//..................................................................................
//..................................................................................


//������ ������������� � ��������������� ������� ����������
//������ �������������� �������
void Permutator_init_opers(struct OpersConfig op)
{
	Permutator_reinit_signs(op);
	Permutator_reinit_pos(op);
}


void Permutator_reinit_signs(struct OpersConfig op)
{
	_Permutator_reinit_signs_part(op.opers, 0, op.opers_size);
}

/*������������� ������� ���������� � ���� [ 1, 2, 3, 4, 5, 6...]
�������� ������������� ����������� ���������������� ��������
(������ ��� �������� ����������� �������� ����� begin).
��� ����� ����� ������������� ������������� ������������������
*/
//WARNING: �� ���������� �������� ��� end <= opers_size
void Permutator_reinit_pos(struct OpersConfig op)
{
	_Permutator_reinit_pos_part_with_zero(op.opers, 0, op.opers_size);
}

//..........................................................................
//..........................................................................
//��������������� ����� �������..

static void _Permutator_reinit_signs_part(struct token* opers, unsigned begin, unsigned end)
{
	for (struct token* i = opers + begin, 
			* const i_end = opers + end;
		i < i_end; ++i)
	{
		i->sign = (OPERATORS)0u;
	}
}

/*������������� ������� ���������� � ���� [ 1, 2, 3, 4, 5, 6...]
�������� ������������� ����������� ���������������� ��������
(������ ��� �������� ����������� �������� ����� begin).
��� ����� ����� ������������� ������������� ������������������
*/
static void _Permutator_reinit_pos_part(struct token* opers, unsigned begin, unsigned end, unsigned min_value)
{
	unsigned val = begin + 1;
	for (struct token* i = opers + begin,
		* const i_end = opers + end;
		i < i_end; ++i, ++val) 
	{
		i->pos = (min_value < val) ? val : min_value;
	}
}

//���������� ������ reinit_pos(begin, end, 0); ������ ��������� � ���� � ������ ������ ���������� - �� ��� ��������
static void _Permutator_reinit_pos_part_with_zero(struct token *opers, unsigned begin, unsigned end)
{
	unsigned val = begin + 1;
	for (struct token* i = opers + begin,
		* const i_end = opers + end;
		i < i_end; ++i, ++val)
	{
		i->pos =  val;
	}
}

//������������ ������� �����.������ �� ��������� ��������� �������.(��� �������� �� �������)
void Permutator_last_pos_configuration(struct OpersConfig config)
{
	for (struct token* i = config.opers,
		*_end = config.opers + config.opers_size;
		i != _end; ++i)
	{
		i->pos = config.opers_size;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

//!!!!!!!!!!!!!!!!!!!!!!���������� �������� ������!!!!!!!!!!!!!!!!!!!!!!!!!!

/*
������ �������� �������� ������ � �� ���������.

��� ����� ����� id �� 0 �� 5.
������� ��� ����� ���� �� ����� ����������, �� ���� ������ ������� ������ ���������� �� 5^(n-1) ��������.
..��� ����� ����������� ��� ��� n ������ � ��� n-1 ��������.
*/

bool Permutator_are_signs_valid(const struct Permutator p)
{
	return p.opers_config.opers->sign < p._WORKING_OPERATORS;
}

//���������� � opers.sign ��������� ������������, �� ��������� �� ����������
/*
������������ ����������� ����������� ������ ������-������. 
����� �������� ����� � i-��� ������� ���������� ����� _WORKING_OPERATORS
� �������� ������������, � �������� ����� ����� ������������� �� �������.
*/
void Permutator_next_sign_configuration(struct Permutator p)
{
	struct token* i = p.opers_config.opers + p.opers_config.opers_size - 1;
	struct token* i_prev = i - 1;
	struct token* const i_end = p.opers_config.opers;
	++(i->sign);
	for (; i != i_end; --i, --i_prev) {
		if (i->sign == p._WORKING_OPERATORS) {
			++(i_prev->sign);
			i->sign = 0;
		} else return;
	}
}

//?????????????????????????????????????????????????????????????????????

/*
����� ����� ������ ���� '������� ������'.

���������� ��� ������� ���������������� ���, ��� ��� ��������� ���������� � ������� : a b + c + ... x + w + z +
[1, 2, 3 ...]
*/

/*
����� �������� ���������� ������� ����������, ��� ���� ��� ��������,
	��� ������� ���� �� ����������,
	��������� �������� ������ ��������� �� ������� n-1
	� ������� ��������� � �������� [i] ���� ���� ����������� ������� ������ i+1
		(����� ���� i�� ���������� ����� ����� ������ ���� i+1 �����)

*/

/*
������������� ������� ������������ "������� ������������ �������".
��� ������-������, ���� ������ ������� ������� ����� �������.
����� �����, ����������� ��� �� �������, � ��� �������� ������ �� ���� ����������.
����� ���������� ��� ������ reinit_pos_part(begin, end, min_value). 
Min_value �����, ����� ��� �������� ������ �� ���� ������ ������������ ���� ��������.

������� ������� ����� ��������� ������
*/

//���������� ���������� ��������� �����������. �� ��������� �� ����������! �� ��������� �� �������!
void Permutator_next_operators_permutation(struct OpersConfig conf)
{
	const unsigned reinit_end = conf.opers_size - 1;
	unsigned reinit_begin = reinit_end;
	
	struct token* i = conf.opers + conf.opers_size - 2;
	struct token* const i_end = conf.opers;

	for (; i > i_end; --i, --reinit_begin) {
		if (i->pos != conf.opers_size) {
			++(i->pos);
			_Permutator_reinit_pos_part(conf.opers, reinit_begin, reinit_end, i->pos);
			return;
		}
	}
	++(conf.opers->pos);
	_Permutator_reinit_pos_part(conf.opers, 1u, reinit_end, conf.opers->pos);
}


//�������� ������� �� ����������
bool Permutator_are_poses_valid(const struct OpersConfig conf)
{
	//�� �� ����� ��������� ��� ������� ���� �� ����������, ��� ��� ��� �������� ������� ��� �������� �����������
	//����� �������� ��� ��� ��������� ������� - ������.
	return (conf.opers)->pos <= conf.opers_size;
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
/*0000000000000000000000000000000000000000000000000000000000000000000*/

                         //������������//

/*
����� ������ ��������, ��� "a b c + +" � "a b + c +" ���������� ��������� ���������� ���������

������� ������� ��� ������� ���������, ����� ����� � ��� �������������� � ��������:
	a b c + +       == a b + c +                        (+-) means +(a) - (b)
	a b c + (+-)    == a b (+-) c (+-)                  (-+) means -(a) + (b) ����� ������������ ��� ~
	a b c + (-+)    == a b (-+) c + == a b (+-) c (-+)
	a b c (+-) +    == a b + c (+-) ------------------------EQUAL 1
	a b c (+-) (+-) == a b (+-) c + == a b (-+) c (-+) -----------EQUAL 2
	a b c (+-) (-+) == a b (-+) c (+-)
	a b c (-+) +    == a b (+-) c + == a b (-+) c (-+) -----------EQUAL 2
	a b c (-+) (+-) == a b + c (+-) ------------------------EQUAL 1
	a b c (-+) (-+) == a b + c (-+)


	����� ��� ������ ������ ������� ������ �� ������ + � - ������ ��������� �� ������ ��������

	a b c * * == a b * c *
	a b c * / == a b / c /
	a b c / * == a b * c /
	a b c / / == a b / c *

	������� ����������� ��������� ��� ������� ����������  ����� ���� ����� ������ �����, �����
	��� ��������� �� ������ ���������(�������� � �������)
	� �� ����� �������� ��� � ��� ���� ��� �������� ����� �������� (�� �������) ��������� ����� ��� �������� ������

	������ ����� ��������� ����� ��������� ������������� �������, ������� ��� �� ����� - ����� �� �������.
*/


/*
����� �������, ��� ������� ������ �������� �� �������!

	��� ��������� ����� �������, ��� ��� �������� ������ ����� ������ ����������� ����������, diff_factor.
	���� ���������� ����� ����������� ������ ����� �������, ��� ����������� �������� ������� ����� ������ ������������.

	������� ��� ����� ���: ���� ����� �� ������ ��������� (+ � *) �� diff_factor ����� ���� �������.
	���� ��� �� ���������� ���������, �� 1 �� ����������� ���� ������� ([-][~] � [~][~]) -
	��� ��������� �� ����� ���� ���� �� �������� ��������, ������� �� ������ ����� 2.
*/


const unsigned diff_factor[OPERATORS_COUNT][OPERATORS_COUNT] =
{
	{ 1, 1, 1, 0, 0 }, // + ��� ����� �� ���� �� ��������� (+-~) ����� diff_factor = 1
	{ 1, 1, 2, 0, 0 }, // - ��� ����� �� ���� �� ��������� ����� ��������� ������. ������������ [-][~] ���������
	{ 1, 1, 2, 0, 0 }, // ~ ��� ����� �� ���� �� ��������� ����� ��������� ������. ������������ [~][~] ���������
	{ 0, 0, 0, 1, 1 }, // * ��� ����� �� ��������� ��������� (*,/) ����� diff_factor = 1
	{ 0, 0, 0, 1, 1 }  // / ��� ����� �� ��������� ��������� (*,/) ����� diff_factor = 1
};



//��������� ��� ������ ������� �� �������� ������� ������ ������� 
bool Permutator_is_doubled(const struct OpersConfig conf)
{
	//���� �� �������� �������� �� ���������� - ������������ false
	if (!Permutator_are_poses_valid(conf)) return false;

	struct token* j = conf.opers + conf.opers_size - 1;
	struct token* i = j - 1;
	struct token* const j_end = conf.opers;
	for (; j != j_end; --i, --j) {
		if (j->pos - i->pos < diff_factor[i->sign][j->sign]) return true;
	}
	return false;
}


/*
//������� ���������� ������� ������� ���������� � �ר��� ������
//��� � ����� � ������ � ������� ����� �� �� ������� ����� ������� � ������ diff_factor.
�������� ���������� Next_Permutation
	������������� ������� ������������ "������� ������������ �������".
	��� ������-������, ���� ������ ������� ������� ����� �������.
	����� �����, ����������� ��� �� �������, � ��� �������� ������ �� ���� ����������.
	����� ���������� ��� ������ minimize_pos(begin, theoretic_min, end).
	�������� ������ ������� ���������� reinit_pos_part, ������ ��� ������ �������� �� ������������.


*/

//�������� �������� � opers ��������� ������������ � ��������� �� �������!
bool Permuator_next_operators_configuration(struct OpersConfig config)
{
	struct token* const opers_last = config.opers + config.opers_size - 1;
	struct token* i = opers_last - 1;
	struct token* j = opers_last;
	struct token* const i_end = config.opers;
	unsigned theoretic_min = config.opers_size;

	for (; i >= i_end; --i, --j, --theoretic_min) {
		unsigned max_pos = j->pos - diff_factor[i->sign][j->sign];
		if (i->pos < max_pos) {
			++(i->pos);
			//���������� ��� ��������� ������� � ������ diff_factor!
			_Permutator_minimize_pos(j, theoretic_min, opers_last);
			return true;
		}
	}
	return false;
}




//�������� ��������� ������(�����������) ����������(�������������) ������������ ������� ����������. ���������� true, ���� �������.
bool Permutator_min_unique_pos(struct OpersConfig conf) {
	struct token* i = conf.opers;
	struct token* j = i + 1;
	struct token* const last = conf.opers + conf.opers_size - 1;
	unsigned num = 2; //theoretical min
	for (i->pos = 1; j <= last; ++i, ++j, ++num) {
		unsigned min_value = i->pos + diff_factor[i->sign][j->sign];
		if (min_value > conf.opers_size) return false;
		j->pos = (min_value > num) ? min_value : num;
	}
	return true;
}

// ������� ���������� min_unique_pos �� ����������[begin, end).������ ��� ������ ������ ������� �� ������� ����� ��� ����������� ��������.
void _Permutator_minimize_pos(struct token* begin, unsigned num, struct token* end)
{
	for (struct token* j = begin,* i = begin - 1; j < end; ++j, ++i, ++num) {
		unsigned min_value = diff_factor[i->sign][j->sign] + i->pos;
		j->pos = (min_value > num) ? min_value : num;
	}
}



//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//������� ����������� ������
void Permutator_print_opers(struct OpersConfig conf)
{
	printf("SIGN : ");
	for (struct token*i = conf.opers, *const _end = conf.opers+conf.opers_size; i != _end; i++) {
		printf("%u ", i->sign);
	}
	printf("\tPOS : ");
	for (struct token* i = conf.opers, * const _end = conf.opers + conf.opers_size; i != _end; i++) {
		printf("%u ", i->pos);
	}
	putchar('\n');
}