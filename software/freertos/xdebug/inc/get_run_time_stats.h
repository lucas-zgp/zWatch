#ifndef _get_run_time_stats_h
#define _get_run_time_stats_h



#include "at32f4xx_conf.h"
#include "at32f4xx.h"
#include "stdio.h"


extern volatile unsigned long long freertos_run_time_ticks;
void ConfigureTimeForRunTimeStats(void);
void RunTimeStats_task_create(void);
#endif




