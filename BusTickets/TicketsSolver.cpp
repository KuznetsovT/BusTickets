#include "TicketsSolver.h"


//Настоятельно рекомендую ознакомиться с объявлением класса.


/*
Задача по решению автобусного билетика заключается в следующем:

	вам даётся n чисел, расставив между ними +-/*() нужно получить goal.
	Переставлять числа запрещается.

	Классически нужно из 6 чисел от 0 до 10 получить 100.
	Пример:  9 9 9 9 9 9 = 100  -> (9/9+9)*(9/9+9) = 100

Решать данную задачу будем используя обратную польскую нотацию.
	В ней вместо "a + b" пишется "a b +", то есть оператор идёт после двух операндов

Задача симметрична относительно знака goal,
	поэтому при попытке передать отрицательный знак
	вернется ошибка invalid_argument.

Аналогично при n = 1 вернется invalid_argument
*/


//--------------------------------------------------------------------




///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////




//!!!!!!!!!!Реализовываем базовые публичные методы!!!!!!!!!!!!!!!



TicketsSolver::TicketsSolver(size_t n, Rational goal, const int* int_data)
	:size(n), opers_size(n - 1), goal(goal)
{
	if (goal.is_negative()) throw std::invalid_argument("TicketsSolver : goal is negative");
	if (size < 2) throw std::invalid_argument("TicketsSolver : size < 2 => no operators");

	data = new Rational[size];
	opers = new token[opers_size];
	eval_list = new Rational[size];
	str_eval_list = new str_token[size];
	init_data(int_data);
	init_opers();
}

//деструктор, удаляет выделенную память
TicketsSolver::~TicketsSolver()
{
	delete[] data;
	delete[] opers;
	delete[] eval_list;
	delete[] str_eval_list;
}


std::string TicketsSolver::first_solution(FLAG notation) noexcept
{

	for (reinit_signs(0, opers_size); are_signs_valid(); next_sign_configuration()) {

		reinit_pos(0, opers_size);
		while (is_doubled()) next_operators_permutation();
		while (are_poses_valid()) {
			if (goal == evaluate()) return str_evaluate(NORMAL_NOTATION);
			do next_operators_permutation(); while (is_doubled());
		}
	
	}
	return std::string();
}

//PROBLEM REWRITE!!!
std::string TicketsSolver::next_solution(FLAG notation) noexcept
{
	if (!are_signs_valid()) return std::string();
	next_operators_permutation();
	do {
		while (is_doubled()) next_operators_permutation();
		while (are_poses_valid()) {
			if (goal == evaluate()) return str_evaluate(NORMAL_NOTATION);		
			do next_operators_permutation(); while (is_doubled());
		}
		next_sign_configuration();
		if (!are_signs_valid()) return std::string();
		reinit_pos(0, opers_size);
	} while (true);

}


void TicketsSolver::all_solutions(std::ostream& out, FLAG notation) noexcept
{
	for (reinit_signs(0, opers_size); are_signs_valid(); next_sign_configuration()) {

		reinit_pos(0, opers_size);
		while (is_doubled()) next_operators_permutation();
		while (are_poses_valid()) {
			if (goal == evaluate()) {
				out << "\t" << goal << " = " << str_evaluate(notation) << std::endl;
			}
			do next_operators_permutation(); while (is_doubled());
		}

	}
}


/*******************************************************************************************/
/*******************************************************************************************/
/*********************************ПРИВАТНЫЕ МЕТОДЫ******************************************/

//контейнер, который сопоставляет оператору выполняющую функцию
const TicketsSolver::func<Rational> TicketsSolver::rational_lib[] = {
	   [](const Rational& a, const Rational& b) {return a + b; },
	   [](const Rational& a, const Rational& b) {return a - b; },
	   [](const Rational& a, const Rational& b) {return -a + b; },
	   [](const Rational& a, const Rational& b) {return a * b; },
	   [](const Rational& a, const Rational& b) {return a / b; }
};



//методы инициализации и реинициализации массива операторов


inline void TicketsSolver::init_data(const int* int_data)
{
	Rational* d = data, *const end = data + size;
	for (const int* i = int_data; d != end; d++, i++) *d = Rational(*i);
}

inline void TicketsSolver::init_opers() noexcept
{
	reinit_signs(0, opers_size);
	reinit_pos(0, opers_size);
}

//часто нам будет требоваться реинициализировать часть массива

//WARNING: не проводится проверки что end <= opers_size
inline void TicketsSolver::reinit_signs(size_t begin, size_t end) noexcept
{
	auto i = opers + begin;
	auto const i_end = opers + end;
	for (; i < i_end; i++) i->sign = OPERATORS(0);
}

//WARNING: не проводится проверки что end <= opers_size

/*инициализация происходит в виде [ 1, 2, 3, 4, 5, 6...] 
Параметр характеризует минимальное инициализируемое значение
(обычно это значение предыдущего элемента перед begin). 
Это нужно чтобы гарантировать неубываемость последовательности
*/
inline void TicketsSolver::reinit_pos(size_t begin, size_t end, const size_t min_value) noexcept
{
	size_t val = begin + 1;
	auto i = opers + begin;
	auto const i_end = opers + end;
	for (; i < i_end; i++, val++) i->pos = std::max(min_value, val);
}


/////////////////////////////////////////////////////////////////////////////////////////////
//Реализация перебора знаков

/*
Опишем алгоритм перебора знаков и их положений.

Все знаки имеют id от 0 до 5.
Считаем что знаки друг от друга независимы, то есть полный перебор знаков проводится за 5^(n-1) операций.
..тут стоит оговориться что при n числах у нас n-1 оператор.
*/


inline bool TicketsSolver::are_signs_valid() const noexcept
{
	return opers->sign < OPERATORS_COUNT;
}


inline void TicketsSolver::next_sign_configuration() noexcept
{
	auto i = opers + opers_size - 1;
	auto i_prev = i - 1;
	auto const i_end = opers;

	i->sign++;
	for (; i != i_end; i--, i_prev--) {
		if (i->sign == OPERATORS_COUNT) {
			i_prev->sign++;
			i->sign = 0;
		} else {
			return;
		}
	}
}


/*
Далее после знаков идут 'позиции знаков'.

Изначально все позиции инициализированы так, что все операторы чередуются с числами: a b + c + ... x + w + z +
	[ 1, 2, 3 ... ]
*/




/*
Далее начинаем перебирать позиции операторов, при этом нам известно,
	что позиции идут по неубыванию,
	последний оператор всегда находится на позиции n-1
	у каждого оператора с индексом [i] есть своя минимальная позиция равная i+1
		(чтобы быть iым оператором перед тобой должно быть i+1 число)

*/

//возвращает формальную следующую престановку. Не проверяет на валидность! Не проверяет на дубляжи!
inline void TicketsSolver::next_operators_permutation() noexcept {

	size_t reinit_begin = opers_size - 1;
	const size_t reinit_end = opers_size - 1;

	auto i = opers + opers_size - 2;
	auto const i_end = opers;
	
	for (; i > i_end; i--, reinit_begin--) {
		if (i->pos != opers_size) {
			i->pos++;
			reinit_pos(reinit_begin, reinit_end, i->pos);
			return;
		}
	}
	opers->pos++;
	reinit_pos(1u, reinit_end, opers->pos);
}

//проверка позиций на валидность
inline bool TicketsSolver::are_poses_valid() const noexcept
{
	//Мы не будем проверять что позиции идут по неубыванию, так как при переборе позиций это свойство сохраняется
	return opers->pos < size;
}

/*
Можно однако заметить, что "a b c + +" и "a b + c +" генерируют абсолютно идентичные выражения

Давайте выпишем все похожие выражения, чтобы найти в них закономерности и отловить:
	a b c + +		== a b + c +						(+-) means +(a) - (b)
	a b c + (+-)	== a b (+-) c (+-)					(-+) means -(a) + (b)
	a b c + (-+)	== a b (-+) c + == a b (+-) c (-+)
	a b c (+-) +    == a b + c (+-) ------------------------EQUAL 1
	a b c (+-) (+-) == a b (+-) c + == a b (-+) c (-+) -----------EQUAL 2
	a b c (+-) (-+) == a b (-+) c (+-)
	a b c (-+) +	== a b (+-) c + == a b (-+) c (-+) -----------EQUAL 2
	a b c (-+) (+-) == a b + c (+-) ------------------------EQUAL 1
	a b c (-+) (-+) == a b + c (-+)


	Видно что первая группа состоит только из знаков + и - Теперь посмотрим на группу уможения

	a b c * * == a b * c *
	a b c * / == a b / c /
	a b c / * == a b * c /
	a b c / / == a b / c *

	Давайте отслеживать операторы что позиции операторов  могут быть равны только тогда, когда
	это операторы из разных вселенных(сложение и деление)
	И не будем забывать что у нас есть два варианта когда соседние (по позиции) операторы имеют два варианта записи

	Данные дубли порождают целое множество эквивалентных решений, которые нам не нужны - нужно их убирать.
*/


/*
Таким образом, нам следует делать проверку на дубляжи!
	Проходим по массиву и смотрим два соседних знака, 
		если у них одинаковые позиции,
			проверяем что они не из одинаковых вселенных(+ и *)
		если у них соседние позиции,
			проверяем что они не относятся к двум описанным выше случаям случаям

*/

//проверяет дубляжи. Если не проходит проверку на валидность - возвращается false
inline bool TicketsSolver::is_doubled() const noexcept
{
	if (!are_poses_valid()) return false;

	auto i = opers + opers_size - 1, j = i - 1;
	auto const i_end = opers;
	for (; i != i_end; i--, j--) {
		if (i->pos == j->pos) 
		{
			if (CheckForSamePos(i, j)) //says that it is a double
				return true;
			
		} else if (i->pos == j->pos + 1) 
		{
			if (CheckForNeighbourPos(i, j)) //says that it is a double
				return true;
		}
	}
	return false;
}

//возвращает является ли данная конфигурация из двух операторов в одном месте дублером
inline bool TicketsSolver::CheckForSamePos(const token* i, const token* j) const noexcept
{
	return (IS_SUMMARY(i->sign) && IS_SUMMARY(j->sign)) || (IS_MULTIPLE(i->sign) && IS_MULTIPLE(j->sign));
}

inline bool TicketsSolver::CheckForNeighbourPos(const token* i, const token* j) const noexcept
{
	return (i->sign == MINUS_PLUS) && (j->sign == MINUS_PLUS || j->sign == MINUS);			
				//почему данные конфигурации - дублеры см. выше
}

//проверяет что оператор принадлежит к классу { +, (+-), (-+) }
inline bool TicketsSolver::IS_SUMMARY(const OPERATORS op) noexcept
{
	return op < MULTIPLE;
}

//проверяет что оператор является оператором умножения-деления
inline bool TicketsSolver::IS_MULTIPLE(const OPERATORS op) noexcept
{
	return !IS_SUMMARY(op);
}


/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

//!!!!!Вычисления!!!!!!!


inline Rational TicketsSolver::evaluate() const noexcept
{
	init_list();
	auto i = opers;
	auto const _end = opers + opers_size;
	for (auto _begin = eval_list; i != _end; i++, _begin++) {
		auto b_iter = eval_list + i->pos;
		auto a_iter = b_iter - 1;
		if (i->sign == DIVIDE && b_iter->IS_NULL()) return Rational::INF;
		*b_iter = rational_lib[i->sign](*a_iter, *b_iter);
		move(a_iter, _begin);
	}
	
	return eval_list[opers_size]; //возвращает последнее оставшееся значение
}

void TicketsSolver::init_list() const noexcept
{
	Rational* i = eval_list;
	Rational* const end = eval_list + size;
	auto j = data;
	for (; i != end; i++, j++) { *i = *j; }
}


void TicketsSolver::move(Rational *a, Rational * const _begin) const noexcept
{
	for (Rational* for_copy = a - 1; for_copy >= _begin; a--, for_copy--) {
		*a = *for_copy;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

//СТРОКОВОЕ ПРЕДСТАВЛЕНИЕ

//при создании REVERSED_NOTATION воспользуемся макросом, так как большая часть кода неудобно повторяется
#define REVERSED_NOTATION_OPERATOR(sign)    []									\
	(const TicketsSolver::str_token& a, const TicketsSolver::str_token& b)		\
	-> TicketsSolver::str_token													\
	{ return { a.str + " " + b.str + " " + sign, TicketsSolver::EXPR }; }


//ОПРЕДЕЛЯЕМ ХРАГИЛИЩЕ ФУНКЦИЙ ДЛЯ ОБРАТНОЙ ПОЛЬСКОЙ НОТАЦИИ 
const TicketsSolver::FLAG TicketsSolver::REVERSED_NOTATION = {
	REVERSED_NOTATION_OPERATOR("+"),
	REVERSED_NOTATION_OPERATOR("(+-)"),
	REVERSED_NOTATION_OPERATOR("(-+)"),
	REVERSED_NOTATION_OPERATOR("*"),
	REVERSED_NOTATION_OPERATOR("/")
};


#undef REVERSED_NOTATION_OPERATOR

#define lambda	[]																\
	(const TicketsSolver::str_token& a, const TicketsSolver::str_token& b)		\
	-> TicketsSolver::str_token
		
//ОПРЕДЕЛЯЕМ ХРАНИЛИЩЕ ФУНКЦИЙ ДЛЯ НОРМАЛЬНОЙ НОТАЦИИ
const TicketsSolver::FLAG TicketsSolver::NORMAL_NOTATION = {
	lambda {
		return { a.str + " + " + b.str, TicketsSolver::SUM};
	},

	lambda {
		if (b.id != TicketsSolver::SUM) return { a.str + " - " + b.str, TicketsSolver::SUM };
		else return { a.str + " -(" + b.str + ")", TicketsSolver::SUM};
	},

	lambda {
		if (a.id != TicketsSolver::SUM) return { "-" + a.str + " + " + b.str, TicketsSolver::SUM };
		else return { "-(" + a.str + ")+ " + b.str, TicketsSolver::SUM};
	},

	lambda {
		str_token t;
		t.str += (a.id == TicketsSolver::MULT || a.id == TicketsSolver::NUM) ? a.str+" " : "(" + a.str + ")";
		t.str += "*";
		t.str += (b.id == TicketsSolver::MULT || b.id == TicketsSolver::NUM) ? " "+b.str : "(" + b.str + ")";
		t.id = TicketsSolver::MULT;
		return t;
	},

	lambda {
		str_token t;
		t.str += (a.id == TicketsSolver::MULT || a.id == TicketsSolver::NUM) ? a.str+" " : "(" + a.str + ")";
		t.str += "/";
		t.str += (b.id == TicketsSolver::NUM) ? " "+b.str : "(" + b.str + ")";
		t.id = TicketsSolver::DIV;
		return t;
	}
};

#undef lambda


//Функция высчиывает строковое представление выражения
inline std::string TicketsSolver::str_evaluate(const FLAG notation) const noexcept
{
	init_str_list();
	auto i = opers;
	auto const _end = opers + opers_size;
	for (auto _begin = str_eval_list; i != _end; i++, _begin++) {
		auto b_iter = str_eval_list + i->pos;
		auto a_iter = b_iter - 1;
		//notation = func<str_token>[]//
		*b_iter = notation[i->sign](*a_iter, *b_iter);
		move(a_iter, _begin);
	}

	return str_eval_list[opers_size].str; //возвращает последнее оставшееся значение
}

//инициализируем лист для вычислений. ОБРАТИТЕ ВНИМАНИЕ - пользуемся Rational::numerator
inline void TicketsSolver::init_str_list() const noexcept
{
	str_token* i = str_eval_list;
	str_token *const end = str_eval_list + size;
	auto j = data;
	for (; i != end; i++, j++) { *i = { std::to_string(j->numer), TicketsSolver::NUM }; }
}

//после вычисления очередного числа нужно сдвинуть элементы в списке
inline void TicketsSolver::move(str_token* a, str_token* const _begin) const noexcept
{
	for (str_token* for_copy = a - 1; for_copy >= _begin; a--, for_copy--) {
		*a = *for_copy;
	}
}