/*
 * scheduler.c
 *
 *  Created on: Nov 15, 2023
 *      Author: Thanh Duy
 */

#ifndef SRC_SCHEDULER_C_
#define SRC_SCHEDULER_C_

#include "scheduler.h"

sTask SCH_tasks_G[SCH_MAX_TASKS];
uint8_t current_index_task = 0;

void SCH_Init(void) {
	current_index_task = 0;
}
void SCH_Add_Task( void(*pFunction)(),
		uint32_t DELAY,
		uint32_t PERIOD) {
	if(current_index_task < SCH_MAX_TASKS) {
		SCH_tasks_G[current_index_task].pTask = pFunction;
		SCH_tasks_G[current_index_task].Delay = DELAY;
		SCH_tasks_G[current_index_task].Period = PERIOD;
		SCH_tasks_G[current_index_task].RunMe = 0;
		SCH_tasks_G[current_index_task].TaskID = current_index_task;

		current_index_task++;
	}
}
void SCH_Update(void){
	for (int i = 0; i < current_index_task;i++) {

		if( SCH_tasks_G[i].Delay > 0) {
			SCH_tasks_G[i].Delay--;
		} else {
			SCH_tasks_G[i].RunMe += 1;
				if (SCH_tasks_G[i].Period) {
					SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period;
				}


		}
	}
}
void SCH_Delete_Task(uint32_t taskID) {
	if(SCH_tasks_G[taskID].pTask == 0) return;
	 for (uint32_t i = taskID; i < SCH_MAX_TASKS - 1; ++i) {
	        SCH_tasks_G[i] = SCH_tasks_G[i + 1];
	    }
	SCH_tasks_G[taskID].pTask = 0x0000;
	SCH_tasks_G[taskID].Delay = 0 ;
	SCH_tasks_G[taskID].Period = 0 ;
	SCH_tasks_G[taskID].RunMe = 0 ;
	current_index_task--;
}
void SCH_Dispatch_Tasks(void) {
	for (int i = 0; i< current_index_task; i++) {
		if(SCH_tasks_G[i].pTask != 0) {
			if (SCH_tasks_G[i].RunMe > 0) {

				(*SCH_tasks_G[i].pTask)();
			}
			if (SCH_tasks_G[i].Period == 0) {
				SCH_Delete_Task(i);
			}
		}
	}
}

#endif /* SRC_SCHEDULER_C_ */
