#include "Evaluator.h"
#include <stdlib.h>


static const OPERATORS
	PLUS = 0u,    //id: 0
	MINUS = 1u,
	MINUS_PLUS = 2u,  //����������� ����: -(a) + b ���������� ~
	MULTIPLE = 3u,
	DIVIDE = 4u; //id: 4


//!!!!!����������!!!!!!!

	//����� ���� ������ ��� ���������� ��������� � ������������� ��� � ��������� ����

//���������� ����� eval_list
static void move(Rational* a, Rational* const _begin);
//�������� �� data � list
static void reinit_eval_list(struct Evaluator eval);

//............................................................................

//���������� ����� ��������� � �������������� �����.

//��������� ������������ �����
typedef struct Rational(*binary_operator_Rational)(struct Rational, struct Rational);

//���������� �������� ����������� ������� �� ���� � ����������� �����������
typedef struct Rational(*safe_binary_operator_Rational)(struct Rational, struct Rational, bool*);

//���������� ���������� � ������, ����� �� ������ ������� �������� ���������
const static binary_operator_Rational honestly_lib[] = {
	Rational_plus,
	Rational_minus,
	Rational_minus_plus,
	Rational_multiple,
	Rational_divide
};

//��������� ��������� ��� �������� � ����������������� ����������


static Rational safe_Rational_plus(Rational, Rational, bool*);
static Rational safe_Rational_minus(Rational, Rational, bool*);
static Rational safe_Rational_minus_plus(Rational, Rational, bool*);
static Rational safe_Rational_multiple(Rational, Rational, bool*);
static Rational safe_Rational_divide(Rational, Rational, bool*);

//���������� ������� ��� ���������������� ����������
const static safe_binary_operator_Rational rational_lib[] = {
	safe_Rational_plus,
	safe_Rational_minus,
	safe_Rational_minus_plus,
	safe_Rational_multiple,
	safe_Rational_divide
};

////////////////////////////////////////////////////////////////////////

//�������� ������ ��� �������������� �����
void* init_Evaluator(struct Evaluator* eval, unsigned size, const Rational* _data, struct OpersConfig *_opers_config)
{
	*eval = (struct Evaluator){ malloc(size * sizeof(Rational)), size, _data, _opers_config };
	return eval->list;
}

//����������� ������ ���������� ��� �����
void destroy_Evaluator(Rational* list)
{
	free(list);
}

//������� ���������� ������ ���������� ���� ���������� ������� �� 0 ���������� INF
	//���� ������������� ��������� ������������� - ���������� -1
Rational Evaluator_evaluate(struct Evaluator eval)
{
	reinit_eval_list(eval);
	struct token* i = eval.opers_config.opers;
	struct token* const _end = eval.opers_config.opers + eval.opers_config.opers_size;

	Rational* _begin = eval.list; //�� ���� ��������� ������ ������� ������� ����������
	for (; i != _end; ++i, ++_begin) {
		Rational* b_iter = eval.list + i->pos;
		Rational* a_iter = b_iter - 1;
		bool flag = true;
		*b_iter = rational_lib[i->sign](*a_iter, *b_iter, &flag);
		//���� ������� �� ����, ���
		//���� ����. ��������� �������������, ������ 
		//���� ���� �� ������ ����� �������� �� ������ ���� �����  - ��������� ������������� ���������
		if (!flag) return *b_iter;
		move(a_iter, _begin);
	}

	return eval.list[eval.opers_config.opers_size]; //���������� ��������� ���������� ��������
}


// ������ �� ����� ���������� ��� ����������
Rational Evaluator_evaluate_honestly(struct Evaluator eval)
{
	reinit_eval_list(eval);
	struct token* i = eval.opers_config.opers;
	struct token* const _end = eval.opers_config.opers + eval.opers_config.opers_size;

	Rational* _begin = eval.list; //�� ���� ��������� ������ ������� ������� ����������
	for (; i != _end; ++i, ++_begin) {
		Rational* b_iter = eval.list + i->pos;
		Rational* a_iter = b_iter - 1;

		*b_iter = honestly_lib[i->sign](*a_iter, *b_iter);
		move(a_iter, _begin);
	}

	return eval.list[eval.opers_config.opers_size]; //���������� ��������� ���������� ��������
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//��������� ������ + ���������� ���������� ��� ���������� ����������.

//���������� ����� eval_list
static void move(Rational* a, Rational* const _begin)
{
	for (Rational* for_copy = a - 1; for_copy >= _begin; --a, --for_copy) {
		*a = *for_copy;
	}
}


//�������� �� data � list
static void reinit_eval_list(struct Evaluator eval)
{
	Rational* i = eval.list;
	Rational* const end = eval.list + eval.list_size;
	const Rational* j = eval.data;
	for (; i != end; ++i, ++j) { *i = *j; }
}

/*************************************************************/

static Rational safe_Rational_plus(Rational a, Rational b, bool* flag)
{
	return Rational_plus(a, b);
}

static Rational safe_Rational_minus(Rational a, Rational b, bool* flag)
{
	if (Rational_less(a, b)) {             //������������� �������� �� ������ ���� �������������� ��. evaluate
		*flag = false;
		return (Rational){ -1, 1 };
	}
	else return Rational_minus(a, b);
}

static Rational safe_Rational_minus_plus(Rational a, Rational b, bool* flag)
{
	if (!(Rational_less(a, b))) {        //������������� �������� �� ������ ���� �������������� ��. evaluate
		*flag = false;
		return (Rational) { -1, 1 };
	}
	else return Rational_minus_plus(a, b);
}


static Rational safe_Rational_multiple(Rational a, Rational b, bool* flag)
{
	return Rational_multiple(a, b);
}

static Rational safe_Rational_divide(Rational a, Rational b, bool* flag)
{
	if (b.numer == 0) {                 //��������� ������� �� ����, ������������� �� ������ ��. evaluate
		*flag = false;
		return INF;
	}
	else return Rational_unsigned_divide(a, b); //�� �������� � �������������� ������� ���� �����������
}
