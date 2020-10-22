#pragma once

#include "Ticket.h"


//далее идут методы для вычисления выражения и представления его в строковом виде

	//реализация класса вычисляющего арифметическое выражение

struct StrConverter {
	struct str_token* list;
	unsigned list_size;
	const struct str_token* str_data;
	struct OpersConfig opers_config;
};


//выделяет достаточно памяти для строкового представления выражения из size чисел
char* static_Str_Converter_Allocate_enough(unsigned size);


//создаёт массив со строковым представлением билетика
struct str_token* static_StrConverter_init_str_data(const unsigned* data, unsigned size);

//конструктор
void* init_Str_Converter(struct StrConverter* sconv, unsigned size, const struct str_token*str_data, struct OpersConfig _opers_config);

//деструктор, удаляющий динамически выделенный list
void destroy_StrConverter(struct str_token* str_list);

//Конвертация арифметического выражения в строку в нормальной нотации
char* StrConverter_normal_convert(struct StrConverter sconv);

//конвертация арифметического выражения в строку в обратной нотации
char* StrConverter_reversed_convert(struct StrConverter sconv);
