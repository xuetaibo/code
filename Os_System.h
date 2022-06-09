/***********************************************************
 * @Author: xuebo
 * @Date: 2022-06-08 08:38:12
 * @LastEditors: xuebo
 * @LastEditTime: 2022-06-09 11:17:21
 * @FilePath: \OS\Os_System.h
 * @Description:
 ***********************************************************/
//
//  Os_System.h
//  OS
//
//  Created by Ѧ�� on 2022/6/4.
//

#ifndef Os_System_h
#define Os_System_h

#include <stdio.h>

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;

//
typedef enum
{
    CPU_ENTER_CRITICAL,
    CPU_EXIT_CRITICAL,
} CPU_EA_TYPEDEF;

//����ص���CPUInterrupt_CallBack_t
typedef void (*CPUInterrupt_CallBack_t)(CPU_EA_TYPEDEF cmd,
                                        uint8_t *pSta);

//ϵͳ����ID
typedef enum
{
    OS_TASK1,
    OS_TASK2,
    OS_TASK3,
    OS_TASK_SUM
} OS_TaskIDTypeDef;

//����״̬
typedef enum
{
    OS_SLEEP,
    OS_RUN = !OS_SLEEP,
} OS_TaskStatusTypeDef;

//ϵͳ����ṹ��
typedef struct
{
    void (*task)(void);           //����ָ��
    OS_TaskStatusTypeDef RunFlag; //����״̬
    uint16_t RunPeriod;           //����ִ������
    uint16_t RunTimer;            //����ִ�м�ʱ��
} OS_TaskTypeDef;

//��������
void OS_CPUInterruptCBSRegister(CPUInterrupt_CallBack_t pCPUInterruptCtrlCBS);
void OS_ClockInterruptHandle(void);
void OS_TaskInit(void);
void OS_CreatTask(OS_TaskIDTypeDef ID,
                  void (*proc)(void),
                  uint16_t TimeDly,
                  OS_TaskStatusTypeDef flag);
void OS_Start(void);
void OS_ClockInterruptHandle(void);
void OS_TaskGetUp(OS_TaskIDTypeDef taskID);
void OS_TaskSleep(OS_TaskIDTypeDef taskID);

#endif /* Os_System_h */
