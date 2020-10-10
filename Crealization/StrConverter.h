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



//создаёт массив со строковым представлением билетика
struct str_token* static_StrConverter_init_str_data(const unsigned* data, unsigned size);

//конструктор
void* init_Str_Converter(struct StrConverter* sconv, unsigned size, const struct str_token*str_data, struct OpersConfig* _opers_config);

//деструктор, удаляющий динамически выделенный list
void destroy_StrConverter(struct str_token* str_list);

