/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*
 * main() creates all the demo application tasks, then starts the scheduler.
 * The web documentation provides more details of the standard demo application
 * tasks, which provide no particular functionality but do provide a good
 * example of how to use the FreeRTOS API.
 *
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
 *
 * "Check" task - This only executes every five seconds but has a high priority
 * to ensure it gets processor time.  Its main function is to check that all the
 * standard demo tasks are still operational.  While no errors have been
 * discovered the check task will print out "OK" and the current simulated tick
 * time.  If an error is discovered in the execution of a task then the check
 * task will print out an appropriate error message.
 *
 */



/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "timers.h"
//#include "queue.h"


//-------------------------------------------------
// Global Variables
//-------------------------------------------------
#define CH3_TASKMANAGEMENT
#define CAPACITY 10
#define APeriod 30
#define BPeriod 70

/* --------------------------------------------- */

//-------------------------------------------------
// Tasks Protopies
//-------------------------------------------------

#ifdef CH3_TASKMANAGEMENT
#if ( configUSE_LST_SCHEDULER == 0 )
void vTask1(void*);
void vTask2(void*);
void vTask3(void*);
void vTask4(void*);
#else
void Task_Exec_A(void*);
void Task_Exec_B(void*);
#endif

#endif





void vApplicationIdleHook(void);

int ANumberOfPeriod = 1;
int BNumberOfPeriod = 1;

int main ( void )
{
#ifdef CH3_TASKMANAGEMENT
	/* Creating Two Task Same Priorities and Delay*/
//	xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
//	xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	/* Creating Two Task Same Priorities and DelayUntil*/
	#if ( configUSE_LST_SCHEDULER == 1 )
	xTaskCreatePeriodic( Task_Exec_A, "Task A", 1000, NULL, 1, NULL, 5, 1 );
	xTaskCreatePeriodic( Task_Exec_B, "Task B", 1000, NULL, 1, NULL, 8, 1 );
	#else
	xTaskCreate( vTask3, "Task 3", 1000, NULL, 1, NULL );
	xTaskCreate( vTask4, "Task 4", 1000, NULL, 1, NULL );
	#endif
#endif

	vTaskStartScheduler();
	return 0;
}

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
 	taskENTER_CRITICAL();
	{
        printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
        fflush(stdout);
	}
	taskEXIT_CRITICAL();
	exit(-1);
}


#ifdef CH3_TASKMANAGEMENT
#if ( configUSE_LST_SCHEDULER == 0 )
void vTask1(void* parameter)
{
    while(1){
        printf("Task 1\n");
		vTaskDelay(pdMS_TO_TICKS(250));
    }
}
void vTask2(void* parameter)
{
    while(1){
        printf("Task 2\n");
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}
void vTask3(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    while(1){
        printf("Task 3 with 250ms\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(250));
    }
}
void vTask4(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    while(1){
        printf("Task 4 with 300ms\n");
        vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(300));
    }
}
#else
void Task_Exec_A(void* parameter)
{
	TickType_t xLastWakeTimeA;
	const TickType_t xFrequency = APeriod; //tsk A frequency
	volatile int count = CAPACITY;     //tsk A capacity
	xLastWakeTimeA = 0;
	while(1)
	{	
		printf("At Time of %04d Ticks-->Task A In with start  %3d\n", xTaskGetTickCount (), APeriod * (ANumberOfPeriod - 1));
		TickType_t xTime = xTaskGetTickCount ();
		TickType_t x;
		while(count != 0)
		{
			if(( x = xTaskGetTickCount () ) > xTime)
			{
				xTime = x;
				count--;
			}
		}
		printf("At Time of %04d Ticks-->Task A Out with deadLine  %3d\n", xTaskGetTickCount (), APeriod* ANumberOfPeriod);
		ANumberOfPeriod += 1;
		count = CAPACITY;
		vTaskDelayUntil( &xLastWakeTimeA, xFrequency );
	}
	
}
void Task_Exec_B(void* parameter)
{
	TickType_t xLastWakeTimeB;
	const TickType_t xFrequency = BPeriod; //tsk B frequency
	volatile int count = CAPACITY;     //tsk B capacity
	xLastWakeTimeB = 0;
	while(1)
	{
		printf("At Time of %04d Ticks-->Task B In with start  %3d\n", xTaskGetTickCount (), BPeriod*(BNumberOfPeriod - 1));
		TickType_t xTime = xTaskGetTickCount ();
		TickType_t x;
		while(count != 0)
		{
			if(( x = xTaskGetTickCount () ) > xTime)
			{
				xTime = x;
				count--;
			}
		}
		printf("At Time of %04d Ticks-->Task B Out with deadLine  %3d\n", xTaskGetTickCount (), BPeriod*BNumberOfPeriod);
		BNumberOfPeriod += 1;
		count = 2*CAPACITY;
		vTaskDelayUntil( &xLastWakeTimeB, xFrequency );
	}
	
}
#endif
#endif
/* CH3_TASKMANAGEMENT ends */


void vApplicationIdleHook(void)
{
//	printf("Idle\r\n");
}
/*-----------------------------------------------------------*/
