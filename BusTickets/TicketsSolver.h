#pragma once

#include <string>
#include <map>
#include <ostream>

#include "Rational.h"



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



class TicketsSolver {
protected:
	//тип - бинарная функция для двух аргументов, например { return a+b; }
	template<class T> using binary_func = T(*)(const T&, const T&);
	//cтруктура для организации отображения решений в str
	struct str_token;

	typedef const binary_func<str_token> FLAG[];
public:

	//используйте данный флаг если нужно вывести выражение в обратной польской нотации
	const static FLAG REVERSED_NOTATION;

	//используйте данный флаг если нужно вывести выражение в нормальной нотации. Стоит по-умолчанию.
	const static FLAG NORMAL_NOTATION;

	//Конструктор класса TicketsSolver. ОБРАТИТЕ ВНИМАНИЕ! goal >= 0 и n > 1 !
	TicketsSolver(unsigned n, /*unsigned*/ Rational goal, const unsigned* data);

	virtual ~TicketsSolver();


	//устанавливает новую цель
	void set_new_goal(Rational goal) noexcept;

	//устанавливает новые числа для решения
	void set_new_data(const unsigned* data);

	//сбрасывает внутренную конфигурацию операторов
	void reset() noexcept;

	//возвращает строковое представление текущей конфигурации операторов
	std::string to_str(const FLAG NOTATION = NORMAL_NOTATION) const noexcept;

	//вычисляет выражение по текущей конфигурации записанной в oper. Если в opers  - фигня, возвращает NaN
	Rational evaluate() const noexcept;
	//если промежуточный результат получается отрицательный возвращает -1, при попытке деления на 0 возвращает INF

	//сбрасывает конфигурацию, возвращает первое найденное решение, если решений нет, возвращает пустую строку
	std::string first_solution(const FLAG notation = NORMAL_NOTATION) noexcept;

	//возвращает словарь всех возможных достижимых goals и количество способов решения
	std::map<Rational, unsigned> map_of_goals() noexcept;

	unsigned count_of_solutions() noexcept;

	//записывает все решения в out, выводит количество записанных решений
	unsigned all_solutions(std::ostream& out, FLAG notation = NORMAL_NOTATION) noexcept;

	//сбрасывает конфигурацию операторов, ищет первое решение, возвращает true если находит
	bool find_first_solution() noexcept;

	//хранит конфигурацию следующего решения, если решений нет,возвращает false
	bool next_solution() noexcept;

protected:
	/*
	* Чтобы не отслеживать унарный минус
	* для простоты используем список из 5 операторов
	* Напоминаем, что goal >= 0, поэтому использование унарного минуса для умножения не нужно.
	*/

	typedef unsigned OPERATORS;
	static constexpr OPERATORS
		PLUS = 0u,    //id: 0
		MINUS = 1u,
		MINUS_PLUS = 2u,  //конструкция вида: -(a) + b обозначаем ~
		MULTIPLE = 3u,
		DIVIDE = 4u;     //id: 4

	//количество знаков в нашей реализации
	static constexpr unsigned OPERATORS_COUNT = 5u;

	//проверяет что op < MULTIPLE
	static constexpr bool IS_SUMMARY(const OPERATORS op) noexcept;
	//проверяет что op >= MULTIPLE
	static constexpr bool IS_MULTIPLE(const OPERATORS op) noexcept;


	/*
	*Всего у нас должно быть (n-1) операторов.
	* У каждого оператора должен быть id который хранится в sign и позиция, которая хранится в pos.
	* Позиция - это номер числа, после которого этот оператор находится, например:
	* "a b с + ..." - позиция оператора "+" равна 2,
	*так как он стоит после третьего числа, а нумерация начинается с нуля
	*/

	//структура описывающая оператор
	struct token {
		OPERATORS sign;
		unsigned pos;
	};

	//каждому строковому представлению мы даём свой id.
	typedef unsigned str_id;

	//определяем какие id мы даём строковым выражениям
	static constexpr str_id
		NUM = 0u,    //число
		SUM = 1u,    //многочлен, сумма нескольких выражений
		MULT = 2u,   //произведение, последний знак - умножение
		DIV = 3u,    //частное, последний знак - деление
		EXPR = 4u;   //сложное выражение (используется в REVERSED_NOTATION)

	

	//строковое представление строим при помощи специальных токенов, где у каждой строки есть свой id
	struct str_token {
		std::string str;
		str_id id = EXPR;
	};

	const unsigned size, opers_size;     //opers_size = size-1
public:	Rational goal;                 //значение которое нужно получить
protected:

	//массив рациональных чисел между которыми нужно расставить знаки арифметических действий
	Rational* data = nullptr;
	//заранее создаём массив в котором хранятся строковые представления чисел из data
	str_token* str_data = nullptr;
	//массив токенов описывающий расстановку знаков в выражении
	token* opers = nullptr;


	//создаём массив data с которым будем работать
	void init_data(const unsigned* int_data);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//ПЕРЕБОР ОПЕРАТОРОВ
public:
	//структура занимающаяся перебором операторов
	struct Permutator {
	private:
		TicketsSolver* ts = nullptr;
	public:
		//конструктор по-умолчанию - бесполезен
		Permutator() = default;
		//связываем с решателем
		Permutator(TicketsSolver* const ts) : ts(ts) {}
		virtual ~Permutator() = default;

		//перед поиском решений производим инициализацию массива
		void init_opers() noexcept;
	private:
		//эти функции проводят частичную реинициализацию массива
		void reinit_signs(const unsigned begin, const unsigned end) noexcept;
		void reinit_pos(const unsigned begin, const unsigned end) noexcept;
		void reinit_pos(const unsigned begin, const unsigned end, const unsigned min_value) noexcept;
	public:
		//полная реиницилизация массива
		void reinit_signs() noexcept;
		void reinit_pos() noexcept;
		
		//пытается построить первую(минимальную) уникальную(недублирующую) конфигурацию позиций операторов. Возвращает true, если успешно.
		bool min_unique_pos() noexcept;
		//переписывает позиции арифм.знаков на последние возможные позиции.(без проверки на дубляжи)
		void last_pos_configuration() noexcept;
	public:
		
		//при обычном переборе используется все 5 знаков
		static constexpr unsigned NORMAL_EVALUATION = OPERATORS_COUNT;
		//если нужен перебор тривиальных решений без деления устанавливайте в OPERATORS_SIZE  = NO_DIVISION
		static constexpr unsigned NO_DIVISION = 4u;
		//если нужен перебор только из плюсов и минусов
		static constexpr unsigned ONLY_SUM = 3u;

		//флаг, показывающий сколько операторов используется в переборе. Для поиска только тривиальных решений используйте NO_DIVISION
		unsigned WORKING_OPERATORS = NORMAL_EVALUATION;

		//cмотрит что настоящая конфигурация знаков валидна, т.е все элементы от 0 до OPERATOR_COUNT
		bool are_signs_valid() const noexcept;

		//Записывает в opers.sign следующую конфигурацию, не проверяет на валидность
		void next_sign_configuration() noexcept;


		//проверка массива позиций на валидность
		bool are_poses_valid() const noexcept;

		//записывает в opers следующую перестановку позиций без проверки на дубляжи!
		void next_operators_permutation() noexcept;

		//пытается записать в opers следующую перестановку с ПРОВЕРКОЙ НА ДУБЛЯЖИ!
		bool next_operators_configuration() noexcept;

		//проверяет что данная позиция не является дублёром другой позиции
		virtual bool is_doubled() const noexcept;
	private:

		/*в массиве содержится информация какое минимально расстояние должно быть между соседними знаками,
		чтобы они не оказались дубляжом другой расстановки знаков*/
		const static unsigned diff_factor[OPERATORS_COUNT][OPERATORS_COUNT];
		//подробнее - см. в cpp-файле "проверка на дубляжи"

		//на отрезке [begin, end) устанавливает минимальную недублированную конфигурацию позиций операторов
		void minimize_pos(TicketsSolver::token* begin, unsigned num, TicketsSolver::token * end) noexcept;

	} permutator; //у каждого TicketsSolver есть свой Permutator


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

	//далее идут методы для вычисления выражения и представления его в строковом виде

	//реализация класса вычисляющего арифметическое выражение
	struct Evaluator {
	private:

		//черновой лист на котором будут происходить вычисления
		mutable Rational* list = nullptr;
		//связь с обьектом TicketsSolver
		TicketsSolver* ts = nullptr;

		//нам нужна бинарная функция, которой будет передоваться флаг проверки на корректность
		typedef Rational(*safe_operator)(const Rational&, const Rational&, bool& flag);
	public:
		//централизованное хранилище для всех нужных нам операторов
		static const safe_operator rational_lib[];

		//контейнер, который сопоставляет оператору выполняющую функцию в случае когда мы честно считаем значение без проверок
		static const binary_func<Rational> honestly_lib[];

		//функция производит нужные вычисления если происходит деление на 0 возвращает INF
		Rational evaluate() const noexcept; //если промежуточный результат отрицательный - возвращает -1

		//честно до конца производит все вычисления
		Rational evaluate_honestly() const noexcept;

		//конструктор по умолчанию - бесполезен
		Evaluator() = default;
		//Конструктор который связывает вычислитель с решателем. Создаёт list который используется как вычислительная доска.
		Evaluator(TicketsSolver* ts);
		//Оператор присваивания, если нужно связать уже существующий вычислитель с решателем.
		Evaluator & operator=(const Evaluator& e);
		virtual ~Evaluator();

	private:
		//копирует из data в list
		void init_list() const noexcept;

		//производим сдвиг eval_list
		static void move(Rational* a, Rational* const _begin) noexcept;

	} evaluator; //у каждого TicketsSolver есть свой Evaluator


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Реализация строкового представления
		//Алгоритм получения строки такой же как и при генерации числа.


public:
	struct StrConverter {
	private:

		//массив на котором будет происходить генерация выходной строки
		mutable TicketsSolver::str_token* str_list = nullptr;
		//связь с обьектом TicketsSolver
		TicketsSolver* ts = nullptr;
	public:

		//метод вычисляющий строковое представление нашей конфигурации
		std::string convert(const FLAG notation = NORMAL_NOTATION) const noexcept;

		
		//конструктор по умолчанию - бесполезен
		StrConverter() = default;
		//конструктор связывает экземпляр с решателем
		StrConverter(TicketsSolver* ts);
		//Связываем уже существующий str_converter с решателем
		StrConverter& operator=(const StrConverter& sc);
		virtual ~StrConverter();

	private:
		//создаёт начальное представление чисел
		void init_str_list() const noexcept;

		//производим сдвиг eval_list
		static void move(str_token* a, str_token* const _begin) noexcept;

	} str_converter;  //у каждого TicketsSolver есть свой StrConverter


};




///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

//макрос который говорит сделать _EXPRESSION_ для каждого решения у обьекта _TS_. 
//При использовании в _EXPRESSION_ оператор return вы получаете только первое решение
#define TS_FOR_ALL_SOLUTIONS(_TS_, _EXPRESSION_)                         \
    for ((_TS_).permutator.reinit_signs();                               \
         (_TS_).permutator.are_signs_valid();                            \
         (_TS_).permutator.next_sign_configuration()) {                  \
                                                                         \
        if ((_TS_).permutator.min_unique_pos()) do {                     \
                                                                         \
            if ((_TS_).goal == (_TS_).evaluator.evaluate()) {            \
                { _EXPRESSION_ }                                         \
            }                                                            \
                                                                         \
        } while ((_TS_).permutator.next_operators_configuration());      \
                                                                         \
    }                                                                    \

