#include "Evaluator.h"
#include <stdlib.h>


static const OPERATORS
	PLUS = 0u,    //id: 0
	MINUS = 1u,
	MINUS_PLUS = 2u,  //конструкция вида: -(a) + b обозначаем ~
	MULTIPLE = 3u,
	DIVIDE = 4u; //id: 4


//!!!!!Вычисления!!!!!!!

	//далее идут методы для вычисления выражения и представления его в строковом виде

//производим сдвиг eval_list
static void move(Rational* a, Rational* const _begin);
//копирует из data в list
static void reinit_eval_list(struct Evaluator eval);

//............................................................................

//Вычисления будем проводить в функциональном стиле.

//операторы рациональных чисел
typedef struct Rational(*binary_operator_Rational)(struct Rational, struct Rational);

//безопасный оператор проверяющий деление на ноль и определённые оптимизации
typedef struct Rational(*safe_binary_operator_Rational)(struct Rational, struct Rational, bool*);

//библиотека операторов в случае, когда мы честно считаем значение выражения
const static binary_operator_Rational honestly_lib[] = {
	Rational_plus,
	Rational_minus,
	Rational_minus_plus,
	Rational_multiple,
	Rational_divide
};

//ОБЪЯВЛЯЕМ ОПЕРАТОРЫ ДЛЯ БЫСТРОГО И ОПТИМИЗИРОВАННОГО ВЫЧИСЛЕНИЯ


static Rational safe_Rational_plus(Rational, Rational, bool*);
static Rational safe_Rational_minus(Rational, Rational, bool*);
static Rational safe_Rational_minus_plus(Rational, Rational, bool*);
static Rational safe_Rational_multiple(Rational, Rational, bool*);
static Rational safe_Rational_divide(Rational, Rational, bool*);

//Библиотека функций для оптимизированных вычислений
const static safe_binary_operator_Rational rational_lib[] = {
	safe_Rational_plus,
	safe_Rational_minus,
	safe_Rational_minus_plus,
	safe_Rational_multiple,
	safe_Rational_divide
};

////////////////////////////////////////////////////////////////////////

//выделяем память для вычислительной доски
void* init_Evaluator(struct Evaluator* eval, unsigned size, const Rational* _data, struct OpersConfig *_opers_config)
{
	*eval = (struct Evaluator){ malloc(size * sizeof(Rational)), size, _data, _opers_config };
	return eval->list;
}

//освобождаем память выделенную для доски
void destroy_Evaluator(Rational* list)
{
	free(list);
}

//функция производит нужные вычисления если происходит деление на 0 возвращает INF
	//если промежуточный результат отрицательный - возвращает -1
Rational Evaluator_evaluate(struct Evaluator eval)
{
	reinit_eval_list(eval);
	struct token* i = eval.opers_config.opers;
	struct token* const _end = eval.opers_config.opers + eval.opers_config.opers_size;

	Rational* _begin = eval.list; //по ходу вчисления начало массива немного сдвигается
	for (; i != _end; ++i, ++_begin) {
		Rational* b_iter = eval.list + i->pos;
		Rational* a_iter = b_iter - 1;
		bool flag = true;
		*b_iter = rational_lib[i->sign](*a_iter, *b_iter, &flag);
		//если деление на ноль, или
		//если пром. результат отрицательный, значит 
		//если один из знаков суммы поменять на другой знак суммы  - получится положительный результат
		if (!flag) return *b_iter;
		move(a_iter, _begin);
	}

	return eval.list[eval.opers_config.opers_size]; //возвращает последнее оставшееся значение
}


// честно до конца производит все вычисления
Rational Evaluator_evaluate_honestly(struct Evaluator eval)
{
	reinit_eval_list(eval);
	struct token* i = eval.opers_config.opers;
	struct token* const _end = eval.opers_config.opers + eval.opers_config.opers_size;

	Rational* _begin = eval.list; //по ходу вчисления начало массива немного сдвигается
	for (; i != _end; ++i, ++_begin) {
		Rational* b_iter = eval.list + i->pos;
		Rational* a_iter = b_iter - 1;

		*b_iter = honestly_lib[i->sign](*a_iter, *b_iter);
		move(a_iter, _begin);
	}

	return eval.list[eval.opers_config.opers_size]; //возвращает последнее оставшееся значение
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//Приватные методы + реализация операторов для безопасной библиотеки.

//производим сдвиг eval_list
static void move(Rational* a, Rational* const _begin)
{
	for (Rational* for_copy = a - 1; for_copy >= _begin; --a, --for_copy) {
		*a = *for_copy;
	}
}


//копирует из data в list
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
	if (Rational_less(a, b)) {             //промежуточные значения не должны быть отрицательными см. evaluate
		*flag = false;
		return (Rational){ -1, 1 };
	}
	else return Rational_minus(a, b);
}

static Rational safe_Rational_minus_plus(Rational a, Rational b, bool* flag)
{
	if (!(Rational_less(a, b))) {        //промежуточные значения не должны быть отрицательными см. evaluate
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
	if (b.numer == 0) {                 //проверяем деление на ноль, дополнительно по флагам см. evaluate
		*flag = false;
		return INF;
	}
	else return Rational_unsigned_divide(a, b); //мы работаем с положительными дробями ради оптимизации
}
