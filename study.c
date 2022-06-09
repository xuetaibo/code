/***********************************************************
 * @Author: xuebo
 * @Date: 2022-06-08 16:04:14
 * @LastEditors: xuebo
 * @LastEditTime: 2022-06-09 10:55:20
 * @FilePath: \OS\study.c
 * @Description:
 ***********************************************************/
#include "stdio.h"
#include "stdlib.h"
#include "Os_System.h"
#include "KeyScan.h"

void OS_TaskInit(void);
void OS_CreatTask(OS_TaskIDTypeDef ID,
                  void (*proc)(void),
                  uint16_t Period,
                  OS_TaskStatusTypeDef flag);
void keyScan();
void OS_Start();

OS_TaskTypeDef OS_Task[OS_TASK_SUM];

int main()
{
   int a, b, c, d;
   a = 12;
   b = 34;
   c = a + b;

   printf("a + b = %d\n", c);

   OS_TaskInit();
   OS_CreatTask(OS_TASK3, keyScan, 500, OS_SLEEP);
   OS_Task[OS_TASK3].RunFlag = OS_RUN;

   for (uint8_t i = 0; i < OS_TASK_SUM; i++)
   {
      printf("OS_Task[%d].task = %d\n", i, OS_Task[i].task);
      printf("OS_Task[%d].RunFlag = %d\n", i, OS_Task[i].RunFlag);
      printf("OS_Task[%d].RunPeriod = %d\n", i, OS_Task[i].RunPeriod);
      printf("OS_Task[%d].RunTimer = %d\n", i, OS_Task[i].RunTimer);
   }

   OS_Start();
}

//------------------------------------------------------------------
//系统任务初始化
void OS_TaskInit(void)
{
   for (uint8_t i = 0; i < OS_TASK_SUM; i++)
   {
      OS_Task[i].task = 0;
      OS_Task[i].RunFlag = OS_SLEEP;
      OS_Task[i].RunPeriod = 0;
      OS_Task[i].RunTimer = 0;
   }
}

//创建系统任务
void OS_CreatTask(OS_TaskIDTypeDef ID,
                  void (*proc)(void),
                  uint16_t Period,
                  OS_TaskStatusTypeDef flag)
{

   if (!OS_Task[ID].task)
   {
      OS_Task[ID].task = proc;
      OS_Task[ID].RunFlag = OS_SLEEP;
      OS_Task[ID].RunPeriod = Period;
      OS_Task[ID].RunTimer = 0;
   }
}

//开始任务
void OS_Start(void)
{
   uint8_t i;
   while (i)
   {
      for (i = 0; i < OS_TASK_SUM; i++)
      {
         if (OS_Task[i].RunFlag == OS_RUN)
         {
            OS_Task[i].RunFlag = OS_SLEEP;
            (*(OS_Task[i].task))();
         }
      }
      i = 0;
   }
}
