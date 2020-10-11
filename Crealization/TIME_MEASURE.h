#pragma once
#include <time.h>
#include <stdio.h>

#define _ms_(time_to_cast) (((time_to_cast)/(double)CLOCKS_PER_SEC)*1000.0)


#define TIME_MEASURE( __code__ , description )                                         \
{                                                                                      \
	double __begin__ = (double) clock(); /*процессорное время*/                        \
	{ __code__ }                                                                       \
	double   __end__ = (double) clock(); /*процессорное время*/                        \
	printf(description);                                                               \
	printf("\tTIME : %.1lf ms\n", _ms_( ( __end__ - __begin__ ) ));                    \
}                                                                                      \



#define TIME_MEASURE_NO_DESCRIPITON( __code__ )                                        \
{                                                                                      \
	double __begin__ = (double) clock(); /*процессорное время*/                        \
	{ __code__ }                                                                       \
	double   __end__ = (double) clock(); /*процессорное время*/                        \
	printf("\tTIME : %.1lf ms\n", _ms_( ( __end__ - __begin__ ) ));                    \
}                                                                                      \



