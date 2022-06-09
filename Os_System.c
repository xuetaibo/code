//
//  Os_System.c
//  OS
//
//  Created by 薛波 on 2022/6/4.
//

#include "Os_System.h"

//定义一个结构体数组OS_Task
volatile OS_TaskTypeDef OS_Task[OS_TASK_SUM];
//定义一个回调函数的指针变量PUInterruptCtrlCBS
CPUInterrupt_CallBack_t CPUInterruptCtrlCBS;

//注册CPU中断控制函数
void OS_CPUInterruptCBSRegister(CPUInterrupt_CallBack_t pCPUInterruptCtrlCBS)
{
    if (CPUInterruptCtrlCBS == 0)
    {
        CPUInterruptCtrlCBS = pCPUInterruptCtrlCBS;
    }
}

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

//系统任务调度函数
void OS_ClockInterrupteHandle(void)
{
    uint8_t i;
    for (i = 0; i < OS_TASK_SUM; i++)
    {
        if (OS_Task[i].task)
        {
            OS_Task[i].RunTimer++;
            if (OS_Task[i].RunTimer > OS_Task[i].RunPeriod)
            {
                OS_Task[i].RunTimer = 0;
                OS_Task[i].RunFlag = OS_RUN;
            }
        }
    }
}

//开始任务
void OS_Start(void)
{
    uint8_t i;
    while (1)
    {
        for (i = 0; i < OS_TASK_SUM; i++)
        {
            if (OS_Task[i].RunFlag == OS_RUN)
            {
                OS_Task[i].RunFlag = OS_SLEEP;
                (*(OS_Task[i].task))();
            }
        }
    }
}

//唤醒一个任务
void OS_TaskGetUp(OS_TaskIDTypeDef taskID)
{
    uint8_t IptStatus;
    if (CPUInterruptCtrlCBS != 0)
    {
        CPUInterruptCtrlCBS(CPU_ENTER_CRITICAL, &IptStatus);
    }
    OS_Task[taskID].RunFlag = OS_RUN;
    if (CPUInterruptCtrlCBS != 0)
    {
        CPUInterruptCtrlCBS(CPU_ENTER_CRITICAL, &IptStatus);
    }
}

//挂起一个任务
void OS_TaskSleep(OS_TaskIDTypeDef taskID)
{
    uint8_t IptStatus;
    if (CPUInterruptCtrlCBS != 0)
    {
        CPUInterruptCtrlCBS(CPU_ENTER_CRITICAL, &IptStatus);
    }
    OS_Task[taskID].RunFlag = OS_SLEEP;
    if (CPUInterruptCtrlCBS != 0)
    {
        CPUInterruptCtrlCBS(CPU_ENTER_CRITICAL, &IptStatus);
    }
}

void LED_flashing()
{
}

/* void main()
{
    //定义一个结构体数组OS_Task
    volatile OS_TaskTypeDef OS_Task[OS_TASK_SUM];
    //定义一个回调函数的指针变量PUInterruptCtrlCBS
    CPUInterrupt_CallBack_t CPUInterruptCtrlCBS;

    //初始化系统任务
    // OS_TaskInit();

    //创建任务
    // OS_CreatTask(OS_TASK3, LED_flashing, 200, OS_SLEEP);
} */