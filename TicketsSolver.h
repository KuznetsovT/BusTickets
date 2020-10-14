#pragma once

#include "Rational.h"
#include "Ticket.h"
#include "Permutator.h"
#include "Evaluator.h"
#include "StrConverter.h"

#include <stdio.h>


//создаём готовую структуру для решения билетика


typedef struct {
	struct Ticket ticket;
	struct OpersConfig opers_config;

	struct Permutator permutator;
	struct Evaluator evaluator;
	struct StrConverter str_converter;
} TicketsSolver;


int init_TicketsSolver(TicketsSolver* ts, unsigned size, int goal,const unsigned *data);

void destroy_TicketsSolver(TicketsSolver* ts);

unsigned count_of_solutions(TicketsSolver ts);

unsigned all_solutions(TicketsSolver  ts, FILE* out);


#define TS_FOR_ALL_SOLUTIONS(__TS__, __EXPRESSION__)                                                      \
{                                                                                                         \
    for (Permutator_reinit_signs((__TS__).permutator.opers_config);                                       \
        Permutator_are_signs_valid((__TS__).permutator);                                                  \
        Permutator_next_sign_configuration((__TS__).permutator))                                          \
        {                                                                                                 \
            Permutator_reinit_pos((__TS__).permutator.opers_config);                                      \
            if (Permutator_min_unique_pos((__TS__).permutator.opers_config)) do {                         \
                                                                                                          \
                if (Rational_equal((__TS__).ticket.goal, Evaluator_evaluate((__TS__).evaluator))) {       \
                    { __EXPRESSION__ }                                                                    \
                }                                                                                         \
                                                                                                          \
            } while (Permuator_next_operators_configuration((__TS__).permutator.opers_config));           \
        }                                                                                                 \
}                                                                                                         \



