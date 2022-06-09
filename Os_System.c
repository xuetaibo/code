//
//  Os_System.c
//  OS
//
//  Created by Ѧ�� on 2022/6/4.
//

#include "Os_System.h"

//����һ���ṹ������OS_Task
volatile OS_TaskTypeDef OS_Task[OS_TASK_SUM];
//����һ���ص�������ָ�����PUInterruptCtrlCBS
CPUInterrupt_CallBack_t CPUInterruptCtrlCBS;

//ע��CPU�жϿ��ƺ���
void OS_CPUInterruptCBSRegister(CPUInterrupt_CallBack_t pCPUInterruptCtrlCBS)
{
    if (CPUInterruptCtrlCBS == 0)
    {
        CPUInterruptCtrlCBS = pCPUInterruptCtrlCBS;
    }
}

//ϵͳ�����ʼ��
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

//����ϵͳ����
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

//ϵͳ������Ⱥ���
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

//��ʼ����
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

//����һ������
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

//����һ������
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
    //����һ���ṹ������OS_Task
    volatile OS_TaskTypeDef OS_Task[OS_TASK_SUM];
    //����һ���ص�������ָ�����PUInterruptCtrlCBS
    CPUInterrupt_CallBack_t CPUInterruptCtrlCBS;

    //��ʼ��ϵͳ����
    // OS_TaskInit();

    //��������
    // OS_CreatTask(OS_TASK3, LED_flashing, 200, OS_SLEEP);
} */