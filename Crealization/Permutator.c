#include "Permutator.h"

static const OPERATORS
	PLUS = 0u,    //id: 0
	MINUS = 1u,
	MINUS_PLUS = 2u,  //конструкция вида: -(a) + b обозначаем ~
	MULTIPLE = 3u,
	DIVIDE = 4u; //id: 4


/*в массиве содержится информация какое минимально расстояние должно быть между соседними знаками,
		чтобы они не оказались дубляжом другой расстановки знаков*/
static const unsigned diff_factor[OPERATORS_COUNT][OPERATORS_COUNT];

//.................................................................................
//.................................................................................

//ОБЪЯВЛЕНИЯ
//методы инициализации и реинициализации массива операторов


//часто нам будет требоваться реинициализировать часть массива
//WARNING: не проводится проверки что end <= opers_size
static void _Permutator_reinit_signs_part(struct Permutator p, unsigned begin, unsigned end);



/*инициализация позиций происходит в виде [ 1, 2, 3, 4, 5, 6...]
Параметр характеризует минимальное инициализируемое значение
(обычно это значение предыдущего элемента перед begin).
Это нужно чтобы гарантировать неубываемость последовательности
*/
//WARNING: не проводится проверки что end <= opers_size
//проверяем, что все элементы после реинициализации не меньше min_value, иначе последовательность не будет неубывающей
static void _Permutator_reinit_pos_part(struct Permutator p, unsigned begin, unsigned end, unsigned min_value);
//аналогично вызову _Permutator_reinit_part(p, begin, end, zero);
static void _Permutator_reinit_pos_part_with_zero(struct Permutator p, const unsigned begin, const unsigned end);



//Следующая функция аналогична _Permutator_reinit_pos_part С УЧЁТОМ ДУБЛИРОВАНИЯ(cм ниже дублирование). 
//num - минимальное теоретическое значение позиции(begin), ведь перед вторым оператором обязано быть два операнда.
//на отрезке [begin, end) устанавливает минимальную недублированную конфигурацию позиций операторов
static void _Permutator_minimize_pos(struct token* begin, unsigned num, struct token* end);

//..................................................................................
//..................................................................................


//методы инициализации и реинициализации массива операторов
//полная реиницилизация массива
void Permutator_init_opers(struct Permutator p)
{
	Permutator_reinit_signs(p);
	Permutator_reinit_pos(p);
}


void Permutator_reinit_signs(struct Permutator p)
{
	_Permutator_reinit_signs_part(p, 0, p.opers_config->opers_size);
}

/*инициализация позиций происходит в виде [ 1, 2, 3, 4, 5, 6...]
Параметр характеризует минимальное инициализируемое значение
(обычно это значение предыдущего элемента перед begin).
Это нужно чтобы гарантировать неубываемость последовательности
*/
//WARNING: не проводится проверки что end <= opers_size
void Permutator_reinit_pos(struct Permutator p)
{
	_Permutator_reinit_pos_part_with_zero(p, 0, p.opers_config->opers_size);
}

//..........................................................................
//..........................................................................
//Реинициализация части массива..

static void _Permutator_reinit_signs_part(struct Permutator p, unsigned begin, unsigned end)
{
	for (struct token* i = p.opers_config->opers + begin, 
			* const i_end = p.opers_config->opers + end;
		i < i_end; ++i)
	{
		i->sign = (OPERATORS)0u;
	}
}

/*инициализация позиций происходит в виде [ 1, 2, 3, 4, 5, 6...]
Параметр характеризует минимальное инициализируемое значение
(обычно это значение предыдущего элемента перед begin).
Это нужно чтобы гарантировать неубываемость последовательности
*/
static void _Permutator_reinit_pos_part(struct Permutator p, unsigned begin, unsigned end, unsigned min_value)
{
	unsigned val = begin + 1;
	for (struct token* i = p.opers_config->opers + begin,
		* const i_end = p.opers_config->opers + end;
		i < i_end; ++i, ++val) 
	{
		i->pos = (min_value < val) ? val : min_value;
	}
}

//аналогично вызову reinit_pos(begin, end, 0); Однако сравнение с нулём в данном случае бесполезно - мы его опускаем
static void _Permutator_reinit_pos_part_with_zero(struct Permutator p, unsigned begin, unsigned end)
{
	unsigned val = begin + 1;
	for (struct token* i = p.opers_config->opers + begin,
		* const i_end = p.opers_config->opers + end;
		i < i_end; ++i, ++val)
	{
		i->pos =  val;
	}
}

//переписывает позиции арифм.знаков на последние возможные позиции.(без проверки на дубляжи)
void Permutator_last_pos_configuration(struct Permutator p)
{
	for (struct token* i = p.opers_config->opers,
		*_end = p.opers_config->opers + p.opers_config->opers_size;
		i != _end; ++i)
	{
		i->pos = p.opers_config->opers_size;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

//!!!!!!!!!!!!!!!!!!!!!!РЕАЛИЗАЦИЯ ПЕРЕБОРА ЗНАКОВ!!!!!!!!!!!!!!!!!!!!!!!!!!

/*
Опишем алгоритм перебора знаков и их положений.

Все знаки имеют id от 0 до 5.
Считаем что знаки друг от друга независимы, то есть полный перебор знаков проводится за 5^(n-1) операций.
..тут стоит оговориться что при n числах у нас n-1 оператор.
*/

bool Permutator_are_signs_valid(const struct Permutator p)
{
	return p.opers_config->opers->sign < p._WORKING_OPERATORS;
}

//Записывает в opers.sign следующую конфигурацию, не проверяет на валидность
/*
Конфигурации постепенным увеличением знаков справа-налево. 
Когда значение знака в i-той позиции становится равно _WORKING_OPERATORS
её занчение сбрасывается, а значение знака слева увеличивается на единицу.
*/
void Permutator_next_sign_configuration(struct Permutator p)
{
	struct token* i = p.opers_config->opers + p.opers_config->opers_size - 1;
	struct token* i_prev = i - 1;
	struct token* const i_end = p.opers_config->opers;
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
Далее после знаков идут 'позиции знаков'.

Изначально все позиции инициализированы так, что все операторы чередуются с числами : a b + c + ... x + w + z +
[1, 2, 3 ...]
*/

/*
Далее начинаем перебирать позиции операторов, при этом нам известно,
	что позиции идут по неубыванию,
	последний оператор всегда находится на позиции n-1
	у каждого оператора с индексом [i] есть своя минимальная позиция равная i+1
		(чтобы быть iым оператором перед тобой должно быть i+1 число)

*/

/*
Реализовываем перебор перестановок "методом постепенного подъёма".
Идём справа-налево, ищем первый элемент который можем поднять.
Когда нашли, увеличиваем его на единицу, а все элементы справа от него сбрасываем.
Сброс происходит при помощи reinit_pos_part(begin, end, min_value). 
Min_value нужно, чтобы все элементы справа не были меньше увеличенного нами элемента.

Нулевой элемент можно увеличить всегда
*/

//возвращает формальную следующую престановку. Не проверяет на валидность! Не проверяет на дубляжи!
void Permutator_next_operators_permutation(struct Permutator p)
{
	const unsigned reinit_end = p.opers_config->opers_size - 1;
	unsigned reinit_begin = reinit_end;
	
	struct token* i = p.opers_config->opers + p.opers_config->opers_size - 2;
	struct token* const i_end = p.opers_config->opers;

	for (; i > i_end; --i, --reinit_begin) {
		if (i->pos != p.opers_config->opers_size) {
			++(i->pos);
			_Permutator_reinit_pos_part(p, reinit_begin, reinit_end, i->pos);
			return;
		}
	}
	++(p.opers_config->opers->pos);
	_Permutator_reinit_pos_part(p, 1u, reinit_end, (p.opers_config->opers)->pos);
}


//проверка позиций на валидность
bool Permutator_are_poses_valid(const struct Permutator p)
{
	//Мы не будем проверять что позиции идут по неубыванию, так как при переборе позиций это свойство сохраняется
	//чтобы показать что все остальные позиции - дублёры.
	return (p.opers_config->opers)->pos <= p.opers_config->opers_size;
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
/*0000000000000000000000000000000000000000000000000000000000000000000*/

                         //ДУБЛИРОВАНИЕ//

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


const unsigned diff_factor[OPERATORS_COUNT][OPERATORS_COUNT] =
{
	{ 1, 1, 1, 0, 0 }, // + все знаки из того же множества (+-~) имеют diff_factor = 1
	{ 1, 1, 2, 0, 0 }, // - все знаки из того же множество имеют ненулевой фактор. конфигурация [-][~] особенная
	{ 1, 1, 2, 0, 0 }, // ~ все знаки из того же множество имеют ненулевой фактор. конфигурация [~][~] особенная
	{ 0, 0, 0, 1, 1 }, // * все знаки из множества умножения (*,/) имеют diff_factor = 1
	{ 0, 0, 0, 1, 1 }  // / все знаки из множества умножения (*,/) имеют diff_factor = 1
};



//проверяет что данная позиция не является дублёром другой позиции 
bool Permutator_is_doubled(const struct Permutator p)
{
	//Если не проходит проверку на валидность - возвращается false
	if (!Permutator_are_poses_valid(p)) return false;

	struct token* j = p.opers_config->opers + p.opers_config->opers_size - 1;
	struct token* i = j - 1;
	struct token* const j_end = p.opers_config->opers;
	for (; j != j_end; --i, --j) {
		if (j->pos - i->pos < diff_factor[i->sign][j->sign]) return true;
	}
	return false;
}


/*
//ФУНКЦИЯ ОПИЫВАЮЩАЯ ПЕРЕБОР ПОЗИЦИЙ ОПЕРАТОРОВ С УЧЁТОМ ДУБЛЕЙ
//идём с конца в начало и смотрим можем ли мы поднять некий элемент с учётом diff_factor.
Действия аналогичны Next_Permutation
	Реализовываем перебор перестановок "методом постепенного подъёма".
	Идём справа-налево, ищем первый элемент который можем поднять.
	Когда нашли, увеличиваем его на единицу, а все элементы справа от него сбрасываем.
	Сброс происходит при помощи minimize_pos(begin, theoretic_min, end).
	Действие данной функции аналогично reinit_pos_part, однако она делает проверку на дублирование.


*/

//пытается записать в opers следующую перестановку с ПРОВЕРКОЙ НА ДУБЛЯЖИ!
bool Permuator_next_operators_configuration(struct Permutator p)
{
	struct token* const opers_last = p.opers_config->opers + p.opers_config->opers_size - 1;
	struct token* i = opers_last - 1;
	struct token* j = opers_last;
	struct token* const i_end = p.opers_config->opers;
	unsigned theoretic_min = p.opers_config->opers_size;

	for (; i >= i_end; --i, --j, --theoretic_min) {
		unsigned max_pos = j->pos - diff_factor[i->sign][j->sign];
		if (i->pos < max_pos) {
			++(i->pos);
			//сбрасываем ужо прошедшие позиции с учётом diff_factor!
			_Permutator_minimize_pos(j, theoretic_min, opers_last);
			return true;
		}
	}
	return false;
}




//пытается построить первую(минимальную) уникальную(недублирующую) конфигурацию позиций операторов. Возвращает true, если успешно.
bool Permutator_min_unique_pos(struct Permutator p) {
	struct token* i = p.opers_config->opers;
	struct token* j = i + 1;
	struct token* const last = p.opers_config->opers + p.opers_config->opers_size - 1;
	unsigned num = 2; //theoretical min
	for (i->pos = 1; j <= last; ++i, ++j, ++num) {
		unsigned min_value = i->pos + diff_factor[i->sign][j->sign];
		if (min_value > p.opers_config->opers_size) return false;
		j->pos = (min_value > num) ? min_value : num;
	}
	return true;
}

// функция аналогична min_unique_pos на промежутке[begin, end).Однако при вызове данной функции мы заранее знаем что минимизация возможна.
void _Permutator_minimize_pos(struct token* begin, unsigned num, struct token* end)
{
	for (struct token* j = begin,* i = begin - 1; j < end; ++j, ++i, ++num) {
		unsigned min_value = diff_factor[i->sign][j->sign] + i->pos;
		j->pos = (min_value > num) ? min_value : num;
	}
}