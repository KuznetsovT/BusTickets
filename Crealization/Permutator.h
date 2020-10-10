#pragma once

#include "Ticket.h"



//Permutator занимается перебором операторов в билетике


struct Permutator {
	struct OpersConfig *opers_config;
	unsigned _WORKING_OPERATORS;
};

//при обычном переборе используется все 5 знаков
const unsigned NORMAL_EVALUATION = OPERATORS_COUNT;
//если нужен перебор тривиальных решений без деления устанавливайте в OPERATORS_SIZE  = NO_DIVISION
const unsigned NO_DIVISION = 4u;
//если нужен перебор только из плюсов и минусов
const unsigned ONLY_SUM = 3u;



/*инициализация позиций происходит в виде [ 1, 2, 3, 4, 5, 6...]
Параметр характеризует минимальное инициализируемое значение
(обычно это значение предыдущего элемента перед begin).
Это нужно чтобы гарантировать неубываемость последовательности
*/


//перед поиском решений производим инициализацию массива. 
void Permutator_init_opers(struct Permutator p);

//полная реиницилизация массива
void Permutator_reinit_signs(struct Permutator p);
void Permutator_reinit_pos(struct Permutator p);

//......................................................
//Выражения "a + b + c" и "(a + b) + c" дублируют друг друга. Подробнее про дублирование см. файл с реализацией.

//проверяет что данная позиция не является дублёром другой позиции 
bool Permutator_is_doubled(const struct Permutator p);

//cмотрит что настоящая конфигурация знаков валидна, т.е все элементы от 0 до OPERATOR_COUNT
bool Permutator_are_signs_valid(const struct Permutator p);

//Записывает в opers.sign следующую конфигурацию, не проверяет на валидность
void Permutator_next_sign_configuration(struct Permutator p);

//проверка массива позиций на валидность
bool Permutator_are_poses_valid(const struct Permutator p);

//записывает в opers формальную следующую перестановку позиций без проверки на дубляжи!
void Permutator_next_operators_permutation(struct Permutator p);

//пытается построить первую(минимальную) уникальную(недублирующую) конфигурацию позиций операторов. Возвращает true, если успешно.
bool Permutator_min_unique_pos(struct Permutator p);
//переписывает позиции арифм.знаков на последние возможные позиции.(без проверки на дубляжи)
void Permutator_last_pos_configuration(struct Permutator p);

//пытается записать в opers следующую перестановку с ПРОВЕРКОЙ НА ДУБЛЯЖИ!
bool Permuator_next_operators_configuration(struct Permutator p);