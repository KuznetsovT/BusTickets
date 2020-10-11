#include "StrConverter.h"
#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>


/*
Технология конвертации выражения в строку аналогична его вычислению.
Находится первый оператор, наодятся два операнда, производится вычисление третьей строки по правилам НОТАЦИИ.
Будем использовать две нотации, NORMAL_NOTATION и REVERSED_NOTATION
*/

static const OPERATORS
	PLUS = 0u,    //id: 0
	MINUS = 1u,
	MINUS_PLUS = 2u,  //конструкция вида: -(a) + b обозначаем ~
	MULTIPLE = 3u,
	DIVIDE = 4u; //id: 4


/*определим количество символов, которое тратится на каждое число.
* Считая, что у нас длина строки пропорциональна количеству битов(в случае когда вывод в 2разрядной системе)
* На скобки и запись знака тратим максимум 16 знаков.
* */
const static unsigned LETTERS_PER_NUMBER = 16 + sizeof(unsigned) * CHAR_BIT;


//определяем какие id мы даём строковым выражениям
static const str_id
	NUM = 0u,    //число
	SUM = 1u,    //многочлен, сумма нескольких выражений
	MULT = 2u,   //произведение, последний знак - умножение
	DIV = 3u,    //частное, последний знак - деление
	EXPR = 4u;   //сложное выражение (используется в REVERSED_NOTATION)

//????????????????????????????????????????????????????????????????
//????????????????????????????????????????????????????????????????

//Обьявления и определения Библиотек Нотаций.
typedef struct str_token(*binary_operator_StrConverter)(struct str_token a, struct str_token b, unsigned str_size);


/*
static struct str_token NORMAL_NOTATION_PLUS(struct str_token a, struct str_token b, unsigned str_size);
static struct str_token NORMAL_NOTATION_MINUS(struct str_token a, struct str_token b, unsigned str_size);
static struct str_token NORMAL_NOTATION_MINUS_PLUS(struct str_token a, struct str_token b, unsigned str_size);
static struct str_token NORMAL_NOTATION_MULTIPLE(struct str_token a, struct str_token b, unsigned str_size);
static struct str_token NORMAL_NOTATION_DIVIDE(struct str_token a, struct str_token b, unsigned str_size);

const binary_operator_StrConverter NORMAL_NOTATION[OPERATORS_COUNT] = {
	NORMAL_NOTATION_PLUS,
	NORMAL_NOTATION_MINUS,
	NORMAL_NOTATION_MINUS_PLUS,
	NORMAL_NOTATION_MULTIPLE,
	NORMAL_NOTATION_DIVIDE
};
*/

//Для обратной нотации с операторами проще,

const char REVERSED_NOTATION_OPERATORS[OPERATORS_COUNT] = {
	'+', //OPERATORS(0)
	'-', //OPERATORS(1)
	'~', //OPERATORS(2) MINUS_PLUS
	'*', //OPERATORS(3)
	'/', //OPERATORS(4)
};


//////////////////////////////////////////////////////////////////
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//первичное заполнение листа строками Копирует строки из str_data
static int _StrConverter_init_Str_List(struct StrConverter sconv);

//производим сдвиг данных в памяти str_list
static void _StrConverter_move(struct str_token* a, struct str_token* const _begin);

//освобождаем выделенную память из str_list на промежутке [begin, end)
static void _StrConverter_destroy_str_list(struct str_token* begin, struct str_token* const end);

///////////////////////////////////////////////////////////////////


//Для обратной нотации конвертация происходит за один проход

char* StrConverter_reversed_convert(struct StrConverter conv) {
	char * const val = malloc(LETTERS_PER_NUMBER * conv.list_size*sizeof(char));
	if (!val) return val;
	char* dest = val;
	struct token* oper = conv.opers_config.opers;
	const struct str_token* data = conv.str_data, * const end = conv.str_data + conv.list_size;
	unsigned num = 0;
	//проходим по всем числам
	for (; data != end; ++data, ++num) {
		//копируем число
		for (const char* source = data->str; *source; ++source, ++dest) *dest = *source;
		*(dest++) = ' ';
		//копируем все идущие после числа операторы
		for (; oper->pos == num; oper++) {
			*(dest++) = REVERSED_NOTATION_OPERATORS[oper->sign];
			*(dest++) = ' ';
		}
	}
	*(dest) = 0;
	return val;
}



//возвращает динамически выделенную строку.
/*char* StrConverter_normal_convert(struct StrConverter conv)
{
	if (_StrConverter_init_Str_List(conv)) { return NULL; }//пытаемся проинициализировать str_list

	const unsigned STR_SIZE = conv.list_size * LETTERS_PER_NUMBER; //количество символов в строках

	struct token* i = conv.opers_config.opers;
	struct token* const _end = conv.opers_config.opers + conv.opers_config.opers_size;

	struct str_token* _begin = conv.list; //по ходу вчисления начало массива немного сдвигается
	for (; i != _end; ++i, ++_begin) {
		struct str_token* b_iter = conv.list + i->pos;
		struct str_token* a_iter = b_iter - 1;

		struct str_token return_val;// = NORMAL_NOTATION[i->sign](*a_iter, *b_iter, STR_SIZE);
		//ГРОМОЗДИМ НУЖНЫЕ НАМ ПРОВЕРКИ.
		if (!return_val.str) { // if success = > return_val != 0
			//_StrConverter_destroy_str_list(_begin, conv.list+conv.list_size);
		}
		free(b_iter->str); free(a_iter->str); //более ненужные строки удаляются по ходу выполнения
		*b_iter = return_val;
		_StrConverter_move(a_iter, _begin);
	}

	return conv.list[conv.opers_config.opers_size].str; //возвращает последнее оставшееся значение
}*/



///////////////////////////////////////////////////////////////////

//создаёт массив со строковым представлением билетика МАССИВ ДИНАМИЧЕСКИЙ - СЛЕДУЕТ ОСВОБОДИТЬ
struct str_token* static_StrConverter_init_str_data(const unsigned* data, unsigned size)
{
	struct str_token* list = malloc(size * sizeof(struct str_token));
	if (!list) return list;

	struct str_token* i_list = list;
	const unsigned * const data_end = data + size;
	for (; data != data_end; ++data, ++i_list)
	{
		*i_list = (struct str_token){ malloc(LETTERS_PER_NUMBER*sizeof(char)), NUM };
		if (i_list) sprintf(i_list->str,"%u", *data);
		else {
			_StrConverter_destroy_str_list(list, i_list);
			free(list);
			return NULL;
		}
	}
	return list;
}


//конструктор
void* init_Str_Converter(struct StrConverter* sconv, unsigned size, const struct str_token* str_data, struct OpersConfig _opers_config)
{
	*sconv = (struct StrConverter){ malloc(size * sizeof(struct str_token)), size, str_data, _opers_config };
	return sconv->list;
}


//деструктор, удаляющий динамически выделенный list
void destroy_StrConverter(struct str_token* str_list) {
	free(str_list);
}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

//первичное заполнение листа строками Копирует строки из str_data
static int _StrConverter_init_Str_List(struct StrConverter conv)
{
	struct str_token* i = conv.list;
	struct str_token* const end = conv.list + conv.list_size;
	const struct str_token* i_data = conv.str_data;

	const size_t STR_SIZE = conv.list_size * LETTERS_PER_NUMBER;
	for (;i != end; ++i, ++i_data)
	{
		i->str = malloc(STR_SIZE*sizeof(char));
		if (!i->str)
		{
			//если выделения памяти не произошло, вызываем аварийное прекращение работы конвертера
			_StrConverter_destroy_str_list(conv.list, i);
			return 1;
		}
		strcpy(i->str, i_data->str);
		i->id = i_data->id;
	}
	return 0;
}


//освобождаем выделенную память из str_list на промежутке [begin, end)
static void _StrConverter_destroy_str_list(struct str_token* a, struct str_token*const end)
{
	for (; a < end; ++a) free(a->str);
}


//производим сдвиг данных в памяти str_list
static void _StrConverter_move(struct str_token* a, struct str_token* const _begin)
{
	for (struct str_token* for_copy = a - 1; for_copy >= _begin; --a, --for_copy) {
		*a = *for_copy;
	}
}



/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/*()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()*/

//РЕАЛИЗАЦИЯ ОПЕРАТОРОВ КОНВЕРСИИ.



#define _REVERSED_NOTATION_OPERATOR( SIGN )                                                              \
{                                                                                                        \
    struct str_token val = { malloc(str_size * LETTERS_PER_NUMBER * sizeof(char)), EXPR };               \
    if (!val.str) return val;                                                                            \
    {                                                                                                    \
        /*val.str = a.str + ' ' + b.str + ' ' + '+';  */                                                 \
        char* dest = val.str;                                                                            \
        for (const char* source = a.str; *source != 0; ++source, ++dest) *dest = *source;                \
        *(dest++) = ' ';                                                                                 \
        for (const char* source = b.str; *source != 0; ++source, ++dest) *dest = *source;                \
        *(dest++) = ' ';                                                                                 \
        *(dest++) = SIGN;                                                                                \
        *dest = 0;                                                                                       \
    }                                                                                                    \
    return val;                                                                                          \
}                                                                                                        \




/////////////////////////////////////////////////////////////////////////////////////////////////

//НОРМАЛЬНАЯ НОТАЦИЯ

static struct str_token NORMAL_NOTATION_PLUS(struct str_token a, struct str_token b, unsigned str_size)
{
	struct str_token val = { malloc(str_size * LETTERS_PER_NUMBER * sizeof(char)), EXPR };
	if (!val.str) return val;


}
static struct str_token NORMAL_NOTATION_MINUS(struct str_token a, struct str_token b, unsigned str_size);
static struct str_token NORMAL_NOTATION_MINUS_PLUS(struct str_token a, struct str_token b, unsigned str_size);
static struct str_token NORMAL_NOTATION_MULTIPLE(struct str_token a, struct str_token b, unsigned str_size);
static struct str_token NORMAL_NOTATION_DIVIDE(struct str_token a, struct str_token b, unsigned str_size);
