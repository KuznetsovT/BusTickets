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



TicketsSolver::TicketsSolver(size_t n, Rational goal, const unsigned* int_data)
	:size(n), opers_size(n - 1), goal(goal)
{
	if (goal.is_negative()) throw std::invalid_argument("TicketsSolver : goal is negative");
	if (size < 2) throw std::invalid_argument("TicketsSolver : size < 2 => no operators");
	if (!goal.IS_NUMBER())throw std::invalid_argument("TicketsSolver: goal is not a number!");

#ifndef _TICKETS_SOLVER_STATIC_ARRAY_SIZE_
	data = new Rational[size];
	opers = new token[opers_size];
	str_data = new str_token[size];
#else
if (size > _TICKETS_SOLVER_STATIC_ARRAY_SIZE_) {
throw std::invalid_argument("length can't be greater than "+std::to_string(_TICKETS_SOLVER_STATIC_ARRAY_SIZE_));
}
#endif
	permutator = Permutator(this);
	init(this, evaluator);
	init(this, str_converter);

	init_data(int_data);
	permutator.init_opers();
}

#ifndef _TICKETS_SOLVER_STATIC_ARRAY_SIZE_

//деструктор, удаляет выделенную память в динамической реализации
TicketsSolver::~TicketsSolver()
{
	delete[] data;
	delete[] opers;
	delete[] str_data;
}

#endif

//устанавливает новую цель, не сбрасывает конфигурацию операторов
void TicketsSolver::set_new_goal(Rational goal) noexcept { this->goal = goal; }

//устанавливает новое значение билетика
void TicketsSolver::set_new_data(const unsigned* data) { init_data(data); }

//сбрасывает конфигурацию операторов, поиск нужно начинать заново=(
void TicketsSolver::reset() noexcept
{
	permutator.init_opers();
}

//используйте, если нужно вывести выражение по хранящейся конфигурации операторов
std::string TicketsSolver::to_str(const TicketsSolver::FLAG NOTATION) noexcept
{
	if (permutator.are_signs_valid() && permutator.are_poses_valid()) return str_converter.convert(NOTATION);
	else return std::string();
}

//используйте, если нужно посчитать выражение по хранящейся конфигурации операторов
Rational TicketsSolver::evaluate() noexcept
{
	if (permutator.are_signs_valid() && permutator.are_poses_valid()) return evaluator.evaluate();
	else return Rational::NaN;
}



//находит первое решение и выводит его строковое представление
std::string TicketsSolver::first_solution(FLAG notation) noexcept
{
	FOR_ALL_SOLUTIONS_(return str_converter.convert();)
		return std::string();
}

/*ищет следующее решение, возвращает true если находит.
ВАЖНО, поиск начинается со следующей конфигурации операторов,
поэтому first_solution !={reset; next_solution},
то есть нулевая конфигурация проверяться не будет
*/
bool TicketsSolver::next_solution() noexcept
{
	if (!permutator.are_signs_valid()) return false;
	permutator.next_operators_permutation();
	do {
		while (permutator.is_doubled()) permutator.next_operators_permutation();
		while (permutator.are_poses_valid()) {
			if (goal == evaluator.evaluate()) return true;
			do permutator.next_operators_permutation(); while (permutator.is_doubled());
		}
		permutator.next_sign_configuration();
		if (!permutator.are_signs_valid()) return false;
		permutator.reinit_pos();
	} while (true);

}

//выводит количество решений выражения
unsigned TicketsSolver::count_of_solutions() noexcept
{
	unsigned count = 0;
	FOR_ALL_SOLUTIONS_(count++;)
		return count;
}

//возвращает множество всех достижимых goals, а так же количество решений
std::map<Rational, unsigned> TicketsSolver::map_of_goals() noexcept
{
	std::map<Rational, unsigned> goals;
	for (permutator.reinit_signs();
		permutator.are_signs_valid();
		permutator.next_sign_configuration()) {
		permutator.reinit_pos();
		while (permutator.is_doubled()) permutator.next_operators_permutation();
		while (permutator.are_poses_valid()) {
			Rational val = evaluator.evaluate();
			if (!val.IS_INF() && !val.is_negative()) goals[val]++;

			do permutator.next_operators_permutation(); while (permutator.is_doubled());
		}
	}
	return goals;
}

//выводит в поток out все нейденные решения
unsigned TicketsSolver::all_solutions(std::ostream& out, FLAG notation) noexcept
{
	unsigned count = 0;
	FOR_ALL_SOLUTIONS_(count++;  out << " " << goal << " = " << str_converter.convert(notation) << std::endl;)
		return count;
}

//возвращает есть ли у билетика решение
bool TicketsSolver::find_first_solution() noexcept
{
	FOR_ALL_SOLUTIONS_(return true;)
		return false;
}


/*******************************************************************************************/
/*******************************************************************************************/
/*********************************ПРИВАТНЫЕ МЕТОДЫ******************************************/



//методы инициализации и реинициализации массива операторов


inline void TicketsSolver::init_data(const unsigned* int_data)
{
	Rational* d = data, * const end = data + size;
	str_token* str_d = str_data;
	for (const auto* i = int_data; d != end; d++, i++, str_d++) {
		*d = Rational(*i);
		*str_d = { std::to_string(*i), TicketsSolver::NUM };
	}
}

inline void TicketsSolver::Permutator::init_opers() const noexcept
{
	reinit_signs(0, ts->opers_size);
	reinit_pos(0, ts->opers_size);
}

//часто нам будет требоваться реинициализировать часть массива

//WARNING: не проводится проверки что end <= opers_size
void TicketsSolver::Permutator::reinit_signs(const size_t begin, const size_t end) const noexcept
{
	auto i = ts->opers + begin;
	auto const i_end = ts->opers + end;
	for (; i < i_end; i++) i->sign = OPERATORS(0);
}

inline void TicketsSolver::Permutator::reinit_signs() const noexcept { reinit_signs(0, ts->opers_size); }

inline void TicketsSolver::Permutator::reinit_pos() const noexcept { reinit_pos(0, ts->opers_size); }

//WARNING: не проводится проверки что end <= opers_size

/*инициализация происходит в виде [ 1, 2, 3, 4, 5, 6...]
Параметр характеризует минимальное инициализируемое значение
(обычно это значение предыдущего элемента перед begin).
Это нужно чтобы гарантировать неубываемость последовательности
*/
void TicketsSolver::Permutator::reinit_pos(const size_t begin, const size_t end, const size_t min_value) const noexcept
{
	size_t val = begin + 1;
	auto i = ts->opers + begin;
	auto const i_end = ts->opers + end;
	for (; i < i_end; i++, val++) i->pos = std::max(min_value, val);
}

//аналогично вызову reinit_pos(begin, end, 0);
void TicketsSolver::Permutator::reinit_pos(const size_t begin, const size_t end) const noexcept
{
	size_t val = begin + 1;
	auto i = ts->opers + begin;
	auto const i_end = ts->opers + end;
	for (; i < i_end; i++, val++) i->pos = val;
}


/////////////////////////////////////////////////////////////////////////////////////////////
//Реализация перебора знаков

/*
Опишем алгоритм перебора знаков и их положений.

Все знаки имеют id от 0 до 5.
Считаем что знаки друг от друга независимы, то есть полный перебор знаков проводится за 5^(n-1) операций.
..тут стоит оговориться что при n числах у нас n-1 оператор.
*/


inline bool TicketsSolver::Permutator::are_signs_valid() const noexcept
{
	return (ts->opers)->sign < OPERATORS_COUNT;
}


inline void TicketsSolver::Permutator::next_sign_configuration() const noexcept
{
	auto i = ts->opers + ts->opers_size - 1;
	auto i_prev = i - 1;
	auto const i_end = ts->opers;

	i->sign++;
	for (; i != i_end; i--, i_prev--) {
		if (i->sign == OPERATORS_COUNT) {
			i_prev->sign++;
			i->sign = 0;
		}
		else {
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
inline void TicketsSolver::Permutator::next_operators_permutation() const noexcept
{
	size_t reinit_begin = ts->opers_size - 1;
	const size_t reinit_end = ts->opers_size - 1;

	auto i = ts->opers + ts->opers_size - 2;
	auto const i_end = ts->opers;

	for (; i > i_end; i--, reinit_begin--) {
		if (i->pos != ts->opers_size) {
			i->pos++;
			reinit_pos(reinit_begin, reinit_end, i->pos);
			return;
		}
	}
	(ts->opers)->pos++;
	reinit_pos(1u, reinit_end, (ts->opers)->pos);
}

//проверка позиций на валидность
inline bool TicketsSolver::Permutator::are_poses_valid() const noexcept
{
	//Мы не будем проверять что позиции идут по неубыванию, так как при переборе позиций это свойство сохраняется
	return (ts->opers)->pos < ts->size;
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
inline bool TicketsSolver::Permutator::is_doubled() const noexcept
{
	if (!are_poses_valid()) return false;

	auto i = ts->opers + ts->opers_size - 1, j = i - 1;
	auto const i_end = ts->opers;
	for (; i != i_end; i--, j--) {
		if (i->pos == j->pos)
		{
			if (CheckForSamePos(i, j)) //says that it is a double
				return true;

		}
		else if (i->pos == j->pos + 1)
		{
			if (CheckForNeighbourPos(i, j)) //says that it is a double
				return true;
		}
	}
	return false;
}

//возвращает является ли данная конфигурация из двух операторов в одном месте дублером
inline bool TicketsSolver::Permutator::CheckForSamePos(const token* i, const token* j) noexcept
{
	return (IS_SUMMARY(i->sign) && IS_SUMMARY(j->sign)) || (IS_MULTIPLE(i->sign) && IS_MULTIPLE(j->sign));
}

inline bool TicketsSolver::Permutator::CheckForNeighbourPos(const token* i, const token* j) noexcept
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

//Определяем массив функций которые будет вызываться вычислителем

//контейнер, который сопоставляет оператору выполняющую функцию
const TicketsSolver::binary_func<Rational> TicketsSolver::Evaluator::rational_lib[] = {
	   [](const Rational& a, const Rational& b) {return a + b; },
	   [](const Rational& a, const Rational& b) {return a - b; },
	   [](const Rational& a, const Rational& b) {return -a + b; },
	   [](const Rational& a, const Rational& b) {return a * b; },
	   [](const Rational& a, const Rational& b) {return a / b; }
};

//связываем вычислитель с решателем и создаём list, использующийся в качестве черновика.
void init(TicketsSolver* ts, TicketsSolver::Evaluator& e)
{
	e.ts = ts;

#ifndef _TICKETS_SOLVER_STATIC_ARRAY_SIZE_
	delete[] e.list;
	e.list = new Rational[ts->size];
#endif
}

#ifndef _TICKETS_SOLVER_STATIC_ARRAY_SIZE_

TicketsSolver::Evaluator::~Evaluator()
{
	delete[] list;
}

#endif


inline Rational TicketsSolver::Evaluator::evaluate() noexcept
{
	init_list();
	auto i = ts->opers;
	auto const _end = ts->opers + ts->opers_size;
	for (auto _begin = list; i != _end; i++, _begin++) {
		auto b_iter = list + i->pos;
		auto a_iter = b_iter - 1;
		if (i->sign == DIVIDE && b_iter->IS_NULL()) return Rational::INF;
		*b_iter = rational_lib[i->sign](*a_iter, *b_iter);
		//если пром. результат отрицательный, значит 
		//если один из знаков суммы поменять на другой знак суммы  - получится положительный результат
		if (b_iter->is_negative()) return Rational(-1);
		move(a_iter, _begin);
	}

	return list[ts->opers_size]; //возвращает последнее оставшееся значение
}

Rational TicketsSolver::Evaluator::evaluate_honestly() noexcept
{
	init_list();
	auto i = ts->opers;
	auto const _end = ts->opers + ts->opers_size;
	for (auto _begin = list; i != _end; i++, _begin++) {
		auto b_iter = list + i->pos;
		auto a_iter = b_iter - 1;

		*b_iter = rational_lib[i->sign](*a_iter, *b_iter);


		move(a_iter, _begin);
	}

	return list[ts->opers_size]; //возвращает последнее оставшееся значение
}


inline void TicketsSolver::Evaluator::init_list() noexcept
{
	Rational* i = list;
	Rational* const end = list + ts->size;
	auto j = ts->data;
	for (; i != end; i++, j++) { *i = *j; }
}


inline void TicketsSolver::Evaluator::move(Rational* a, Rational* const _begin) noexcept
{
	for (Rational* for_copy = a - 1; for_copy >= _begin; a--, for_copy--) {
		*a = *for_copy;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

//СТРОКОВОЕ ПРЕДСТАВЛЕНИЕ

//Определяем массивы функций которые будут вызываться конвертёром

//при создании REVERSED_NOTATION воспользуемся макросом, так как большая часть кода неудобно повторяется
#define REVERSED_NOTATION_OPERATOR(sign)    []									\
	(const TicketsSolver::str_token& a, const TicketsSolver::str_token& b)		\
	-> TicketsSolver::str_token													\
	{ return { a.str + " " + b.str + " " + sign, TicketsSolver::EXPR }; }


//ОПРЕДЕЛЯЕМ ХРАГИЛИЩЕ ФУНКЦИЙ ДЛЯ ОБРАТНОЙ ПОЛЬСКОЙ НОТАЦИИ 
const TicketsSolver::binary_func<TicketsSolver::str_token> TicketsSolver::REVERSED_NOTATION[] = {
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
const TicketsSolver::binary_func<TicketsSolver::str_token> TicketsSolver::NORMAL_NOTATION[] = {
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
		t.str += (a.id == TicketsSolver::MULT || a.id == TicketsSolver::NUM) ? a.str + " " : "(" + a.str + ")";
		t.str += "*";
		t.str += (b.id == TicketsSolver::MULT || b.id == TicketsSolver::NUM) ? " " + b.str : "(" + b.str + ")";
		t.id = TicketsSolver::MULT;
		return t;
	},

	lambda {
		str_token t;
		t.str += (a.id == TicketsSolver::MULT || a.id == TicketsSolver::NUM) ? a.str + " " : "(" + a.str + ")";
		t.str += "/";
		t.str += (b.id == TicketsSolver::NUM) ? " " + b.str : "(" + b.str + ")";
		t.id = TicketsSolver::DIV;
		return t;
	}
};

#undef lambda

//связываем вычислитель с решателем и создаём str_list, использующийся в качестве черновика.
void init(TicketsSolver* ts, TicketsSolver::StrConverter& sc)
{
	sc.ts = ts;
#ifndef _TICKETS_SOLVER_STATIC_ARRAY_SIZE_

	delete[] sc.str_list;
	sc.str_list = new TicketsSolver::str_token[ts->size];
#endif
}

#ifndef _TICKETS_SOLVER_STATIC_ARRAY_SIZE_

TicketsSolver::StrConverter::~StrConverter()
{
	delete[] str_list;
}

#endif

//Функция высчиывает строковое представление выражения
inline std::string TicketsSolver::StrConverter::convert(const TicketsSolver::FLAG NOTATION) noexcept
{
	init_str_list();
	auto i = ts->opers;
	auto const _end = ts->opers + ts->opers_size;
	for (auto _begin = str_list; i != _end; i++, _begin++) {
		auto b_iter = str_list + i->pos;
		auto a_iter = b_iter - 1;
		//NOTATION = binary_func<str_token>[]//
		*b_iter = NOTATION[i->sign](*a_iter, *b_iter);
		move(a_iter, _begin);
	}

	return str_list[ts->opers_size].str; //возвращает последнее оставшееся значение
}


//инициализируем лист для вычислений. ОБРАТИТЕ ВНИМАНИЕ - пользуемся Rational::numerator
inline void TicketsSolver::StrConverter::init_str_list() noexcept
{
	str_token* i = str_list;
	str_token* const end = str_list + ts->size;
	auto j = ts->str_data;
	for (; i != end; i++, j++) *i = *j;
}

//после вычисления очередного числа нужно сдвинуть элементы в списке
inline void TicketsSolver::StrConverter::move(str_token* a, str_token* const _begin) noexcept
{
	for (str_token* for_copy = a - 1; for_copy >= _begin; a--, for_copy--) {
		*a = *for_copy;
	}
}