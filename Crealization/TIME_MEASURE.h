#pragma once
#include <time.h>
#include <stdio.h>

#define _ms_(time_to_cast) (((time_to_cast)/(double)CLOCKS_PER_SEC)*1000.0)

#define TIME_MEASURE_BEGIN(identifier)                                                 \
double identifier = 0;                                                                 \
{                                                                                      \
    double __begin__ = (double) clock();                                               \
    {                                                                                  \




#define TIME_MEASURE_END(identifier)                                                   \
        }                                                                              \
    double   __end__ = (double) clock();                                               \
    identifier = _ms_( ( __end__ - __begin__ ) );                                      \
}                                                                                      \





#define TIME_MEASURE( __code__ , description )                                         \
{                                                                                      \
	TIME_MEASURE_BEGIN (diff_time_measured)                                            \
	{ __code__ }                                                                       \
	TIME_MEASURE_END(diff_time_meassured)                                              \
	printf(description);                                                               \
	printf("\tTIME : %.1lf ms\n", diff_time_meassured);                                \
}                                                                                      \



#define TIME_MEASURE_NO_DESCRIPTION( __code__ )                                        \
{                                                                                      \
	TIME_MEASURE_BEGIN(diff_time_meassured)                                            \
	{ __code__ }                                                                       \
	TIME_MEASURE_END(diff_time_meassured)                                              \
	printf("\tTIME : %.1lf ms\n", diff_time_meassured);                                \
}                                                                                      \



