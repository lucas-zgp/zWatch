#include "get_run_time_stats.h"


#include "FreeRTOS.h"
#include "task.h"

#include "usart.h"
#include "my_assert.h"
#include "string.h"

volatile unsigned long long freertos_run_time_ticks;

static void timer6_proxy_init()
{
  RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR6 , ENABLE);
  TMR_TimerBaseInitType  TMR_TimeBaseStructure = {0};
  TMR_TimeBaseStructInit(&TMR_TimeBaseStructure);
  TMR_TimeBaseStructure.TMR_DIV = 50-1;
  TMR_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;
  TMR_TimeBaseStructure.TMR_Period = 240-1;
  TMR_TimeBaseStructure.TMR_ClockDivision = TMR_CKD_DIV1;

  TMR_TimeBaseInit(TMR6,&TMR_TimeBaseStructure);
  NVIC_InitType NVIC_InitStructure = {0};
  NVIC_InitStructure.NVIC_IRQChannel = TMR6_GLOBAL_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  
  TMR_INTConfig(TMR6, TMR_INT_Overflow, ENABLE);
  TMR_Cmd(TMR6, ENABLE);
}


void TMR6_GLOBAL_IRQHandler(void)
{
  if(TMR_GetINTStatus(TMR6,TMR_INT_Overflow) == SET) 
  {
    freertos_run_time_ticks++;
  }
  TMR_ClearITPendingBit(TMR6, TMR_INT_Overflow);
}



void ConfigureTimeForRunTimeStats(void)
{
  freertos_run_time_ticks=0;
  timer6_proxy_init();	/*初始化定时器6*/
}


char RunTimeInfo[800];		//保存任务运行时间信息

void RunTimeStats_task(void *pvParameters)
{
  while(1)
  {
    memset(RunTimeInfo,0,800);				//信息缓冲区清零
    vTaskGetRunTimeStats(RunTimeInfo);		//获取任务运行时间信息
    printf("\r\ntask_name\t\t\ttime\tpercent\r\n");
    printf("%s\r\n",RunTimeInfo);

    vTaskDelay(1000);                           	//延时10ms，也就是1000个时钟节拍	
  }
}

TaskHandle_t RunTimeStats_Handler;
void RunTimeStats_task_create()
{
  xTaskCreate((TaskFunction_t )RunTimeStats_task,     
                (const char*    )"b",   
                (uint16_t       )256,
                (void*          )NULL,
                (UBaseType_t    )configMAX_PRIORITIES,
                (TaskHandle_t*  )&RunTimeStats_Handler); 

  if(RunTimeStats_Handler == NULL)
  {
    my_assert(0,__FILE__,__LINE__);
  }         
  else
  {

  }
}


