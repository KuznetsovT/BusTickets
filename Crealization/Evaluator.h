#pragma once

#include "Ticket.h"

//!!!!!Вычисления!!!!!!!

//далее идут методы для вычисления выражения и представления его в строковом виде

	//реализация класса вычисляющего арифметическое выражение

struct Evaluator {
	Rational* list;   //используется в качестве рассчётной доски
	unsigned list_size;
	const Rational* data;
	struct OpersConfig opers_config;
};

//конструктор
void* init_Evaluator(struct Evaluator *eval, unsigned size, const Rational*_data, struct OpersConfig *_opers_config);
//деструктор, удаляющий динамически выделенный list
void destroy_Evaluator(Rational *list);

//оптимизированное вычисление арифметич. выражения. 
//При делении на ноль возвращает INF, при отрицательном промежуточном результате - (-1)
Rational Evaluator_evaluate(struct Evaluator eval);

//честно считает арифметическое выражение
Rational Evaluator_evaluate_honestly(struct Evaluator eval);