#ifndef _query_task_stats_task_h
#define _query_task_stats_task_h



#include "at32f4xx_conf.h"
#include "at32f4xx.h"
#include "stdio.h"


extern volatile unsigned long long freertos_run_time_ticks;

void query_task_stats_task_create(void);
void ConfigureTimeForRunTimeStats(void);

void RunTimeStats_task_create(void);

#endif




