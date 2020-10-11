#pragma once

#include "Ticket.h"


//далее идут методы дл€ вычислени€ выражени€ и представлени€ его в строковом виде

	//реализаци€ класса вычисл€ющего арифметическое выражение

struct StrConverter {
	struct str_token* list;
	unsigned list_size;
	const struct str_token* str_data;
	struct OpersConfig opers_config;
};





//создаЄт массив со строковым представлением билетика
struct str_token* static_StrConverter_init_str_data(const unsigned* data, unsigned size);

//конструктор
void* init_Str_Converter(struct StrConverter* sconv, unsigned size, const struct str_token*str_data, struct OpersConfig _opers_config);

//деструктор, удал€ющий динамически выделенный list
void destroy_StrConverter(struct str_token* str_list);

// онвертаци€ арифметического выражени€ в строку в нормальной нотации
char* StrConverter_normal_convert(struct StrConverter sconv);

//конвертаци€ арифметического выражени€ в строку в обратной нотации
char* StrConverter_reversed_convert(struct StrConverter sconv);
