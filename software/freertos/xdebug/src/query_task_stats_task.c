#include "query_task_stats_task.h"

#include "FreeRTOS.h"
#include "task.h"

#include "usart.h"
#include "my_assert.h"
#include "gpio.h"
#include "message_transmit_task.h"
#include "common.h"

char InfoBuffer[2048]; //保存信息的数组

void query_task_stats_task(void *pvParameters)
{
  while (1)
  {
    // ArraySize=uxTaskGetNumberOfTasks();		//获取系统任务数量
    // StatusArray=pvPortMalloc(ArraySize*sizeof(TaskStatus_t));//申请内存
    // if(StatusArray!=NULL)					//内存申请成功
    // {
    //   ArraySize=uxTaskGetSystemState((TaskStatus_t* )StatusArray, 	//任务信息存储数组
    //                                  (UBaseType_t		)ArraySize, 	  //任务信息存储数组大小
    //                                  (uint32_t*		  )&TotalRunTime);//保存系统总的运行时间
    //   printf("\r\nTaskName\t\tPriority\t\tTaskNumber\t\t\r\n");
    //   for(x=0;x<ArraySize;x++)
    //   {
    //     //通过串口打印出获取到的系统任务的有关信息，比如任务名称、
    //     //任务优先级和任务编号。
    //     printf("%s\t\t%d\t\t\t%d\t\t\t\r\n",
    //         StatusArray[x].pcTaskName,
    //         (int)StatusArray[x].uxCurrentPriority,
    //         (int)StatusArray[x].xTaskNumber);

    //   }
    // }
    // vPortFree(StatusArray);	//释放内存

    taskENTER_CRITICAL();

    printf("%s\r\n", memory_information_data.factory_information_data.Product_id);
    printf("%s\r\n", memory_information_data.factory_information_data.Hardware_model);
    printf("%d\r\n", memory_information_data.system_parameter_data.breath_threshold);

    printf("/*************************************************/\r\n");
    vTaskList(InfoBuffer);        //获取所有任务的信息
    printf("%s\r\n", InfoBuffer); //通过串口打印所有任务的信息
    printf("/*************************************************/\r\n");
    taskEXIT_CRITICAL();

    vTaskDelay(5000);
  }
}

TaskHandle_t query_task_stats_task_handler;
void query_task_stats_task_create()
{
  xTaskCreate((TaskFunction_t)query_task_stats_task,
              (const char *)"query_task_stats_task",
              (uint16_t)1024,
              (void *)NULL,
              (UBaseType_t)configMAX_PRIORITIES,
              (TaskHandle_t *)&query_task_stats_task_handler);

  if (query_task_stats_task_handler == NULL)
  {
    my_assert(0, __FILE__, __LINE__);
  }
  else
  {
  }
}
