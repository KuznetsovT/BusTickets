#pragma once

#include "Ticket.h"



//Permutator занимается перебором операторов в билетике


struct Permutator {
	struct OpersConfig opers_config;
	unsigned _WORKING_OPERATORS;
};

//при обычном переборе используется все 5 знаков
extern const unsigned NORMAL_EVALUATION;
//если нужен перебор тривиальных решений без деления устанавливайте в OPERATORS_SIZE  = NO_DIVISION
extern const unsigned NO_DIVISION;
//если нужен перебор только из плюсов и минусов
extern const unsigned ONLY_SUM;



/*инициализация позиций происходит в виде [ 1, 2, 3, 4, 5, 6...]
Параметр характеризует минимальное инициализируемое значение
(обычно это значение предыдущего элемента перед begin).
Это нужно чтобы гарантировать неубываемость последовательности
*/


//перед поиском решений производим инициализацию массива. 
void Permutator_init_opers(struct OpersConfig op);

//полная реиницилизация массива
void Permutator_reinit_signs(struct OpersConfig conf);
void Permutator_reinit_pos(struct OpersConfig conf);

//......................................................
//Выражения "a + b + c" и "(a + b) + c" дублируют друг друга. Подробнее про дублирование см. файл с реализацией.

//проверяет что данная позиция не является дублёром другой позиции 
bool Permutator_is_doubled(const struct OpersConfig conf);

//cмотрит что настоящая конфигурация знаков валидна, т.е все элементы от 0 до OPERATOR_COUNT
bool Permutator_are_signs_valid(const struct Permutator p);

//Записывает в opers.sign следующую конфигурацию, не проверяет на валидность
void Permutator_next_sign_configuration(struct Permutator p);

//проверка массива позиций на валидность
bool Permutator_are_poses_valid(const struct OpersConfig conf);

//записывает в opers формальную следующую перестановку позиций без проверки на дубляжи!
void Permutator_next_operators_permutation(struct OpersConfig conf);

//пытается построить первую(минимальную) уникальную(недублирующую) конфигурацию позиций операторов. Возвращает true, если успешно.
bool Permutator_min_unique_pos(struct OpersConfig conf);
//переписывает позиции арифм.знаков на последние возможные позиции.(без проверки на дубляжи)
void Permutator_last_pos_configuration(struct OpersConfig conf);

//пытается записать в opers следующую перестановку с ПРОВЕРКОЙ НА ДУБЛЯЖИ!
bool Permuator_next_operators_configuration(struct OpersConfig conf);

//функция отладочного вывода
void Permutator_print_opers(struct OpersConfig conf);
