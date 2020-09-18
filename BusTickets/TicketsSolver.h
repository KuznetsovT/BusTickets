#pragma once

#include <string>


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
private:
	//тип - бинарная функция для двух аргументов, например { return a+b; }
	template<class T> using binary_func = T(*)(const T&, const T&);
	//чтруктура для организации отображения решений в str
	struct str_token;
public:
	typedef const binary_func<str_token> FLAG[];

	//используйте данный флаг если нужно вывести выражение в обратной польской нотации
	const static FLAG REVERSED_NOTATION;

	//используйте данный флаг если нужно вывести выражение в нормальной нотации. Стоит по-умолчанию.
	const static FLAG NORMAL_NOTATION;

	//Конструктор класса TicketsSolver. ОБРАТИТЕ ВНИМАНИЕ! goal >= 0 и n > 1 !
	TicketsSolver(size_t n, /*unsigned*/ Rational goal, const unsigned *data);
	
	virtual ~TicketsSolver();
	
	//сбрасывает конфигурацию, возвращает первое найденное решение, если решений нет, возвращает пустую строку
	std::string first_solution(FLAG notation = NORMAL_NOTATION) noexcept;
	//возвращает следующее решение, хранит получившуюся конфигурацию, если решений нет, возвращает пустую строку
	std::string next_solution(FLAG notation = NORMAL_NOTATION) noexcept;
	
	void all_solutions(std::ostream& out, FLAG notation = NORMAL_NOTATION)noexcept;

private:
	//сбрасывает конфигурацию операторов, ищет первое решение, возвращает true если находит
	bool find_first_solution() noexcept;


	void all_solutions();

	/*
	* Чтобы не отслеживать унарный минус
	* для простоты используем список из 5 операторов
	* Напоминаем, что goal >= 0, поэтому использование унарного минуса для умножения не нужно.
	*/

	typedef unsigned OPERATORS;
	static const OPERATORS
		PLUS = 0u,    //id: 0
		MINUS = 1u,
		MINUS_PLUS = 2u,  //конструкция вида: -(a) + b
		MULTIPLE = 3u,
		DIVIDE = 4u;     //id: 4
	static const unsigned OPERATORS_COUNT = 5u;
	//проверяет что op < MULTIPLE
	static bool IS_SUMMARY(const OPERATORS op) noexcept;
	//проверяет что op >= MULTIPLE
	static bool IS_MULTIPLE(const OPERATORS op) noexcept;


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
		size_t pos;
	};


	const size_t size, opers_size; //opers_size = size-1
	Rational goal;                 //значение которое нужно получить

	//массив рациональных чисел между которыми нужно расставить знаки арифметических действий
	Rational* data = nullptr;
	
	//массив токенов описывающий расстановку знаков в выражении
	token* opers =  nullptr;

	//создаём массив data с которым будем работать
	void init_data(const unsigned* int_data);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//ПЕРЕБОР ОПЕРАТОРОВ

	//структура занимающаяся перебором операторов
	struct Permutator {
	private:
		TicketsSolver* ts = nullptr;
	public:
		Permutator() = default;
		Permutator(TicketsSolver* const ts) : ts(ts) {}

		//перед поиском решений производим инициализацию массива
		void init_opers() const noexcept;

		//эти функции проводят частичную реинициализацию массива
		void reinit_signs(const size_t begin, const size_t end) const noexcept;
		void reinit_pos(const size_t begin, const size_t end) const noexcept;
	private:
		void reinit_pos(const size_t begin, const size_t end, const size_t min_value) const noexcept;

	public:
		//cмотрит что настоящая конфигурация знаков валидна, т.е все элементы от 0 до OPERATOR_COUNT
		bool are_signs_valid() const noexcept;
		//Записывает в opers.sign следующую конфигурацию, не проверяет на валидность
		void next_sign_configuration() const noexcept;

		//проверка массива позиций на валидность
		bool are_poses_valid()const noexcept;

		//записывает в signs следующую перестановку позиций с проверкой на дубляжи!
		void next_operators_permutation() const noexcept;

		//проверяет что данная позиция не является дублёром другой позиции
		virtual bool is_doubled() const noexcept;
	private:
		//вспомогательные функции для is_doubled
		static bool CheckForSamePos(const token* i, const token* j) noexcept;
		static bool CheckForNeighbourPos(const token* i, const token* j) noexcept;

	} permutator; //у каждого TicketsSolver есть свой Permutator


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

	//далее идут методы для вычисления выражения и представления его в строковом виде
	
	//реализация класса вычисляющего арифметическое выражение
	struct Evaluator {
	private:
		//черновой лист на котором будут происходить вычисления
		Rational* list = nullptr;
		TicketsSolver* ts = nullptr;
	public:
		//централизованное хранилище для всех нужных нам операторов
		static const binary_func<Rational> rational_lib[];

		//функция производит нужные вычисления если происходит деление на 0 возвращает INF
		Rational evaluate() const noexcept; //если промежуточный результат отрицательный - возвращает -1
		
		//связывает Evaluator и TicketsSolver а так же производит выделение памяти по лист
		void init(TicketsSolver* ts);
		virtual ~Evaluator();

	private:
		//копирует из data в eval_list
		void init_list() const noexcept;

		//производим сдвиг eval_list
		static void move(Rational* a, Rational* const _begin) noexcept;

	} evaluator; //у каждого TicketsSolver есть свой Evaluator

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Реализация строкового представления
	//Алгоритм получения строки такой же как и при генерации числа.

	//у каждого выражения есть свой id
	typedef unsigned str_id;

	//определяем какие id мы даём строковым выражениям
	static const str_id
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

	struct StrConverter {
	private:
		//массив на котором будет происходить генерация выходной строки
		TicketsSolver::str_token* str_list = nullptr;
		//связь с обьектом TicketsSolver
		TicketsSolver* ts = nullptr;
	public:

		//метод вычисляющий строковое представление нашей конфигурации
		std::string convert(const FLAG notation = NORMAL_NOTATION) const noexcept;

		//cвязываем обьект типа StrConverter с обьектом типа TicketsSolver
		void init(TicketsSolver* ts);
		~StrConverter();
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
///////////////////////////////////////////////////////////////////////////////////////////////

