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

//макрос для написания методов вида: сделай expression для каждого решения /*чтобы получить первое решение, достаточно вставить return в expression*/
#define FOR_ALL_SOLUTIONS_(__EXPRESSION__)                                                  \
{                                                                                           \
                                                                                            \
    for (permutator.reinit_signs();                                                         \
        permutator.are_signs_valid();                                                       \
        permutator.next_sign_configuration()) {                                             \
                                                                                            \
        if (permutator.min_unique_pos()) do {                                               \
                                                                                            \
            if (goal == evaluator.evaluate()) { __EXPRESSION__ }                            \
                                                                                            \
        } while (permutator.next_operators_configuration());                                \
                                                                                            \
    }                                                                                       \
}                                                                                           \


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////




//!!!!!!!!!!Реализовываем базовые публичные методы!!!!!!!!!!!!!!!



TicketsSolver::TicketsSolver(unsigned n, Rational goal, const unsigned* int_data)
	:size(n), opers_size(n - 1), goal(goal)
{
	if (goal.is_negative()) throw std::invalid_argument("TicketsSolver : goal is negative");
	if (size < 2) throw std::invalid_argument("TicketsSolver : size < 2 => no operators");
	if (!goal.IS_NUMBER())throw std::invalid_argument("TicketsSolver: goal is not a number!");


	data = new Rational[size];
	opers = new token[opers_size];
	str_data = new str_token[size];

	permutator = Permutator(this);
	evaluator = Evaluator(this);
	str_converter = StrConverter(this);

	init_data(int_data);
	permutator.init_opers();
}



//деструктор, удаляет выделенную память в динамической реализации
TicketsSolver::~TicketsSolver()
{
	delete[] data;
	delete[] opers;
	delete[] str_data;
}



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
std::string TicketsSolver::to_str(const TicketsSolver::FLAG NOTATION) const noexcept
{
	if (permutator.are_signs_valid() && permutator.are_poses_valid()) return str_converter.convert(NOTATION);
	else return std::string();
}

//используйте, если нужно посчитать выражение по хранящейся конфигурации операторов
Rational TicketsSolver::evaluate() const noexcept
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
	if (!permutator.are_signs_valid()) return false;                                  //START CONDITIONS
	if (!permutator.is_doubled()) {                                                   //
	    if (!permutator.next_operators_configuration()) goto sign_increase;           //
	} else {                                                                          //
	    do permutator.next_operators_permutation(); while (permutator.is_doubled());  //
	    if (!permutator.are_poses_valid()) goto sign_increase;                        //
	}
	do {                                                        
		do {                                                    //POS INCREASE
		    if (goal == evaluator.evaluate()) return true;      //
		} while (permutator.next_operators_configuration());    //
		                                                        
	sign_increase:   //GOTO SIGN INCREASE
		permutator.next_sign_configuration();                   //SIGN INCREASE
		if (!permutator.are_signs_valid()) return false;        //
		if (!permutator.min_unique_pos()) goto sign_increase;
	} while (true);
}

//выводит количество решений выражения
unsigned TicketsSolver::count_of_solutions() noexcept
{
	unsigned count = 0;
	FOR_ALL_SOLUTIONS_(++count;)
	return count;
}

//возвращает множество всех достижимых goals, а так же количество решений
std::map<Rational, unsigned> TicketsSolver::map_of_goals() noexcept
{
	std::map<Rational, unsigned> goals;
	for (permutator.reinit_signs(); permutator.are_signs_valid(); 
		permutator.next_sign_configuration()) {
                    
		if (permutator.min_unique_pos()) do {
			Rational val = evaluator.evaluate();
			if (!val.IS_INF() && !val.is_negative()) ++goals[val];

		} while (permutator.next_operators_configuration());

	}
	return goals;
}

//выводит в поток out все нейденные решения
unsigned TicketsSolver::all_solutions(std::ostream& out, FLAG notation) noexcept
{
	unsigned count = 0;
	FOR_ALL_SOLUTIONS_(
		count++;
		out << " " << goal << " = " << str_converter.convert(notation) << std::endl; 
	)
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


void TicketsSolver::init_data(const unsigned* int_data)
{
	Rational* d = data, * const end = data + size;
	str_token* str_d = str_data;
	for (const auto* i = int_data; d != end; ++d, ++i, ++str_d) {
		*d = Rational(*i);
		*str_d = { std::to_string(*i), TicketsSolver::NUM };
	}
}

void TicketsSolver::Permutator::init_opers() noexcept
{
	reinit_signs(0, ts->opers_size);
	reinit_pos(0, ts->opers_size);
}

//часто нам будет требоваться реинициализировать часть массива

//WARNING: не проводится проверки что end <= opers_size
void TicketsSolver::Permutator::reinit_signs(const unsigned begin, const unsigned end) noexcept
{
	auto i = ts->opers + begin;
	auto const i_end = ts->opers + end;
	for (; i < i_end; ++i) i->sign = OPERATORS(0);
}

void TicketsSolver::Permutator::reinit_signs() noexcept { reinit_signs(0, ts->opers_size); }

void TicketsSolver::Permutator::reinit_pos() noexcept { reinit_pos(0, ts->opers_size); }

//WARNING: не проводится проверки что end <= opers_size

/*инициализация позиций происходит в виде [ 1, 2, 3, 4, 5, 6...]
Параметр характеризует минимальное инициализируемое значение
(обычно это значение предыдущего элемента перед begin).
Это нужно чтобы гарантировать неубываемость последовательности
*/
void TicketsSolver::Permutator::reinit_pos(const unsigned begin, const unsigned end, const unsigned min_value) noexcept
{
	size_t val = begin + 1;
	auto i = ts->opers + begin;
	auto const i_end = ts->opers + end;
	for (; i < i_end; ++i, ++val) i->pos = (min_value < val) ? val : min_value;
}

//аналогично вызову reinit_pos(begin, end, 0);
void TicketsSolver::Permutator::reinit_pos(const unsigned begin, const unsigned end) noexcept
{
	size_t val = begin + 1;
	auto i = ts->opers + begin;
	auto const i_end = ts->opers + end;
	for (; i < i_end; ++i, ++val) i->pos = val;
}


void TicketsSolver::Permutator::last_pos_configuration() noexcept
{
	for (token* i = ts->opers, *_end = ts->opers + ts->opers_size; i != _end; ++i) i->pos = ts->opers_size;
}

/////////////////////////////////////////////////////////////////////////////////////////////

//!!!!!!!!!!!!!!!!!!!!!!РЕАЛИЗАЦИЯ ПЕРЕБОРА ЗНАКОВ!!!!!!!!!!!!!!!!!!!!!!!!!!

/*
Опишем алгоритм перебора знаков и их положений.

Все знаки имеют id от 0 до 5.
Считаем что знаки друг от друга независимы, то есть полный перебор знаков проводится за 5^(n-1) операций.
..тут стоит оговориться что при n числах у нас n-1 оператор.
*/


bool TicketsSolver::Permutator::are_signs_valid() const noexcept
{
	return (ts->opers)->sign < WORKING_OPERATORS;
}



void TicketsSolver::Permutator::next_sign_configuration() noexcept
{
	auto i = ts->opers + ts->opers_size - 1;
	auto i_prev = i - 1;
	auto const i_end = ts->opers;

	++(i->sign);
	for (; i != i_end; --i, --i_prev) {
		if (i->sign == WORKING_OPERATORS) {
			++(i_prev->sign);
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
void TicketsSolver::Permutator::next_operators_permutation() noexcept
{
	size_t reinit_begin = ts->opers_size - 1;
	const size_t reinit_end = ts->opers_size - 1;

	auto i = ts->opers + ts->opers_size - 2;
	auto const i_end = ts->opers;

	for (; i > i_end; --i, --reinit_begin) {
		if (i->pos != ts->opers_size) {
			++(i->pos);
			reinit_pos(reinit_begin, reinit_end, i->pos);
			return;
		}
	}
	++(ts->opers->pos);
	reinit_pos(1u, reinit_end, (ts->opers)->pos);
}


//проверка позиций на валидность
bool TicketsSolver::Permutator::are_poses_valid() const noexcept
{
	//Мы не будем проверять что позиции идут по неубыванию, так как при переборе позиций это свойство сохраняется
	//однако часть функций может специально устанавливать opers->pos равным ts->size 
	//чтобы показать что все остальные позиции - дублёры.
	return (ts->opers)->pos < ts->size;
}


/*
Можно однако заметить, что "a b c + +" и "a b + c +" генерируют абсолютно идентичные выражения

Давайте выпишем все похожие выражения, чтобы найти в них закономерности и отловить:
	a b c + +       == a b + c +                        (+-) means +(a) - (b) 
	a b c + (+-)    == a b (+-) c (+-)                  (-+) means -(a) + (b) может обозначаться как ~
	a b c + (-+)    == a b (-+) c + == a b (+-) c (-+)
	a b c (+-) +    == a b + c (+-) ------------------------EQUAL 1
	a b c (+-) (+-) == a b (+-) c + == a b (-+) c (-+) -----------EQUAL 2
	a b c (+-) (-+) == a b (-+) c (+-)
	a b c (-+) +    == a b (+-) c + == a b (-+) c (-+) -----------EQUAL 2
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
	
	Для краткости можно сказать, что для соседних знаков можно ввести минимальное расстояние, diff_factor.
	если расстояние между операторами меньше этого фактора, они обязательно окажутся дублёром некой другой конфигурации.

	Описать это можно так: если знаки из разных вселенных (+ и *) то diff_factor между ними нулевой.
	Если они из одинаковых вселенных, то 1 за исключением двух случаев ([-][~] и [~][~]) - 
	эти операторы не могут быть даже на соседних позициях, поэтому их фактор равен 2.
*/


const unsigned TicketsSolver::Permutator::diff_factor[TicketsSolver::OPERATORS_COUNT][TicketsSolver::OPERATORS_COUNT] =
{
	{ 1, 1, 1, 0, 0 }, // + все знаки из того же множества (+-~) имеют diff_factor = 1
	{ 1, 1, 2, 0, 0 }, // - все знаки из того же множество имеют ненулевой фактор. конфигурация [-][~] особенная
	{ 1, 1, 2, 0, 0 }, // ~ все знаки из того же множество имеют ненулевой фактор. конфигурация [~][~] особенная
	{ 0, 0, 0, 1, 1 }, // * все знаки из множества умножения (*,/) имеют diff_factor = 1
	{ 0, 0, 0, 1, 1 }  // / все знаки из множества умножения (*,/) имеют diff_factor = 1
};


//проверяет дубляжи. Если не проходит проверку на валидность - возвращается false
bool TicketsSolver::Permutator::is_doubled() const noexcept
{
	if (!are_poses_valid()) return false;

	auto j = ts->opers + ts->opers_size - 1, i = j - 1;
	auto const j_end = ts->opers;
	for (; j != j_end; --i, --j) {
		if (j->pos - i->pos < diff_factor[i->sign][j->sign]) return true;
	}
	return false;
}




//ФУНКЦИЯ ОПИЫВАЮЩАЯ ПЕРЕБОР ПОЗИЦИЙ ОПЕРАТОРОВ С УЧЁТОМ ДУБЛЕЙ
//идём с конца в начало и смотрим можем ли мы поднять некий элемент с учётом diff_factor.
bool TicketsSolver::Permutator::next_operators_configuration() noexcept
{
	token * const opers_last = ts->opers + ts->opers_size - 1;
	token*  i = opers_last - 1;
	token* j = opers_last;
	auto const i_end = ts->opers;
	unsigned num = ts->opers_size;

	for (; i >= i_end; --i, --j, --num) {
		unsigned max_pos = j->pos - diff_factor[i->sign][j->sign];
		if (i->pos < max_pos) {
			++(i->pos);
			//сбрасываем ужо прошедшие позиции с учётом diff_factor!
			minimize_pos(j, num, opers_last); 
			return true;
		}
	}
	return false;
}


//пытается составить минимальную уникальную конфигурацию позиций операторов. Если удаётся, возвращает true
bool TicketsSolver::Permutator::min_unique_pos() noexcept
{
	token * i = ts->opers;
	token * j = i + 1;
	token *const last = ts->opers + ts->opers_size -1;
	unsigned num = 2;
	for (i->pos = 1; j <= last; ++i, ++j, ++num) {
		unsigned min_value = i->pos + diff_factor[i->sign][j->sign];
		if (min_value > ts->opers_size) return false;
		j->pos = (min_value > num) ? min_value : num;
	}
	return true;
}

//функция аналогична min_unique_pos на промежутке [begin, end). Однако при вызове данной функции мы заранее знаем что минимизация возможна.
void TicketsSolver::Permutator::minimize_pos(TicketsSolver::token* begin, unsigned num, TicketsSolver::token* end) noexcept
{
	for(auto j = begin, i = begin-1; j<end; ++j, ++i, ++num) {
		unsigned min_value = diff_factor[i->sign][j->sign] + i->pos;
		j->pos = (min_value > num) ? min_value : num;
	}
}


//проверяет что оператор принадлежит к классу { +, (+-), (-+) }
constexpr bool TicketsSolver::IS_SUMMARY(const OPERATORS op) noexcept
{
	return op < MULTIPLE;
}

//проверяет что оператор является оператором умножения-деления
constexpr bool TicketsSolver::IS_MULTIPLE(const OPERATORS op) noexcept
{
	return !IS_SUMMARY(op);
}


/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

//!!!!!Вычисления!!!!!!!

//Определяем массив функций которые будет вызываться вычислителем

//контейнер, который сопоставляет оператору выполняющую функцию. Проверяем дополнительные условия.
const TicketsSolver::Evaluator::safe_operator TicketsSolver::Evaluator::rational_lib[] = {
		[](const Rational& a, const Rational& b, bool& flag) {
			return a + b; 
		},
		[](const Rational& a, const Rational& b, bool& flag) { 
			if (a < b) { 
				flag = false;       //промежуточные значения не должны быть отрицательными см. evaluate
				return Rational(-1); 
			} else return a - b; 
		},
		[](const Rational& a, const Rational& b, bool& flag) { 
			if (!(a < b)) { 
				flag = false;       //промежуточные значения не должны быть отрицательными см. evaluate
				return Rational(-1); 
			} else return -a + b; 
		},
		[](const Rational& a, const Rational& b, bool& flag) {
			return a * b;
		},
		[](const Rational& a, const Rational& b, bool& flag) { 
			if (0 == b.numer) { 
				flag = false;       //проверяем деление на ноль, дополнительно по флагам см. evaluate
				return Rational::INF; 
			} else return a / b;
		}
};



//контейнер, который сопоставляет оператору выполняющую функцию в случае когда мы честно считаем значение без проверок
const TicketsSolver::binary_func<Rational> TicketsSolver::Evaluator::honestly_lib[] = {
		[](const Rational& a, const Rational& b) { return  a + b; },
		[](const Rational& a, const Rational& b) { return  a - b; },
		[](const Rational& a, const Rational& b) { return -a + b; },
		[](const Rational& a, const Rational& b) { return  a * b; },
		[](const Rational& a, const Rational& b) { return  a / b; }
};


//Конструктор связывающий вычислитель с решателем и выделяющий память под вычислительную доску(list)
TicketsSolver::Evaluator::Evaluator(TicketsSolver* ts) : ts(ts)
{
	list = new Rational[ts->size];
}

//Оператор присваивания, если нужно связать уже существующий вычислитель с решателем.
TicketsSolver::Evaluator& TicketsSolver::Evaluator::operator=(const Evaluator& e)
{
	this->ts = e.ts;

	delete[] this->list;
	this->list = new Rational[this->ts->size];

	return *this;
}

TicketsSolver::Evaluator::~Evaluator()
{
	delete[] list;
	list = nullptr;
}


//функция производит нужные вычисления если происходит деление на 0 возвращает INF
Rational TicketsSolver::Evaluator::evaluate() const noexcept  //если промежуточный результат отрицательный - возвращает -1
{
	init_list();
	auto i = ts->opers;
	auto const _end = ts->opers + ts->opers_size;
	for (auto _begin = list; i != _end; ++i, ++_begin) {
		auto b_iter = list + i->pos;
		auto a_iter = b_iter - 1;
		bool flag = true;
		*b_iter = rational_lib[i->sign](*a_iter, *b_iter, flag);
		//если деление на ноль, или
		//если пром. результат отрицательный, значит 
		//если один из знаков суммы поменять на другой знак суммы  - получится положительный результат
		if (!flag) return *b_iter;
		move(a_iter, _begin);
	}

	return list[ts->opers_size]; //возвращает последнее оставшееся значение
}

//честно до конца производит все вычисления
Rational TicketsSolver::Evaluator::evaluate_honestly() const noexcept
{
	init_list();
	auto i = ts->opers;
	auto const _end = ts->opers + ts->opers_size;
	for (auto _begin = list; i != _end; ++i, ++_begin) {
		auto b_iter = list + i->pos;
		auto a_iter = b_iter - 1;
		bool flag = true;
		*b_iter = honestly_lib[i->sign](*a_iter, *b_iter);


		move(a_iter, _begin);
	}

	return list[ts->opers_size]; //возвращает последнее оставшееся значение
}


void TicketsSolver::Evaluator::init_list() const noexcept
{
	Rational* i = list;
	Rational* const end = list + ts->size;
	auto j = ts->data;
	for (; i != end; ++i, ++j) { *i = *j; }
}


void TicketsSolver::Evaluator::move(Rational* a, Rational* const _begin) noexcept
{
	for (Rational* for_copy = a - 1; for_copy >= _begin; --a, --for_copy) {
		*a = *for_copy;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

//СТРОКОВОЕ ПРЕДСТАВЛЕНИЕ

//Определяем массивы функций которые будут вызываться конвертёром

//при создании REVERSED_NOTATION воспользуемся макросом, так как большая часть кода неудобно повторяется
#define REVERSED_NOTATION_OPERATOR(sign)    []                                  \
    (const TicketsSolver::str_token& a, const TicketsSolver::str_token& b)      \
    -> TicketsSolver::str_token                                                 \
    { return { a.str + " " + b.str + " " + sign, TicketsSolver::EXPR }; }       \



//ОПРЕДЕЛЯЕМ ХРАНИЛИЩЕ ФУНКЦИЙ ДЛЯ ОБРАТНОЙ ПОЛЬСКОЙ НОТАЦИИ 
const TicketsSolver::binary_func<TicketsSolver::str_token> TicketsSolver::REVERSED_NOTATION[] = {
	REVERSED_NOTATION_OPERATOR("+"),
	REVERSED_NOTATION_OPERATOR("-"),
	REVERSED_NOTATION_OPERATOR("~"),          //знак минус-плюс
	REVERSED_NOTATION_OPERATOR("*"),
	REVERSED_NOTATION_OPERATOR("/"),
};


#undef REVERSED_NOTATION_OPERATOR

#define lambda  []                                                              \
    (const TicketsSolver::str_token& a, const TicketsSolver::str_token& b)      \
    -> TicketsSolver::str_token                                                 \



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
TicketsSolver::StrConverter::StrConverter(TicketsSolver* ts) : ts(ts)
{
	str_list = new str_token[ts->size];
}

//Связываем уже существующий str_converter с решателем
TicketsSolver::StrConverter& TicketsSolver::StrConverter::operator=(const StrConverter& sc)
{
	this->ts = sc.ts;
	delete[] str_list;
	str_list = new TicketsSolver::str_token[ts->size];
	return *this;
}

TicketsSolver::StrConverter::~StrConverter()
{
	delete[] str_list;
	str_list = nullptr;
}


//Функция высчиывает строковое представление выражения
std::string TicketsSolver::StrConverter::convert(const TicketsSolver::FLAG NOTATION) const noexcept
{
	init_str_list();
	auto i = ts->opers;
	auto const _end = ts->opers + ts->opers_size;
	for (auto _begin = str_list; i != _end; ++i, ++_begin) {
		auto b_iter = str_list + i->pos;
		auto a_iter = b_iter - 1;
		//NOTATION = binary_func<str_token>[]//
		*b_iter = NOTATION[i->sign](*a_iter, *b_iter);
		move(a_iter, _begin);
	}

	return str_list[ts->opers_size].str; //возвращает последнее оставшееся значение
}


//инициализируем лист для вычислений. ОБРАТИТЕ ВНИМАНИЕ - пользуемся Rational::numerator
void TicketsSolver::StrConverter::init_str_list() const noexcept
{
	str_token* i = str_list;
	str_token* const end = str_list + ts->size;
	auto j = ts->str_data;
	for (; i != end; ++i, ++j) *i = *j;
}

//после вычисления очередного числа нужно сдвинуть элементы в списке
void TicketsSolver::StrConverter::move(str_token* a, str_token* const _begin) noexcept
{
	for (str_token* for_copy = a - 1; for_copy >= _begin; --a, --for_copy) {
		*a = *for_copy;
	}
}