#include "StrConverter.h"


#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>


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





///////////////////////////////////////////////////////////////////

//создаёт массив со строковым представлением билетика
struct str_token* static_StrConverter_init_str_data(const unsigned* data, unsigned size)
{
	struct str_token* list = malloc(size * sizeof(struct str_token));
	if (!list) return list;

	struct str_token* i_list = list;
	const unsigned* i_data = data, * const data_end = data + size;
	for (; i_data != data_end; ++data, ++i_list)
	{
		*i_list = (struct str_token){ malloc(LETTERS_PER_NUMBER * size), NUM };
		sprintf(i_list->str, "%u", *i_data);
	}
	return list;
}


//конструктор
void* init_Str_Converter(struct StrConverter* sconv, unsigned size, const struct str_token* str_data, struct OpersConfig* _opers_config)
{
	*sconv = (struct StrConverter){ malloc(size * sizeof(struct str_token)), size, str_data, _opers_config };
	if (!sconv->list) return sconv->list;
	for (struct str_token* i = sconv->list, * const _end = sconv->list + size; i != _end; ++i)
	{

	}
	return sconv->list;
}