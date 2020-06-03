/*
 * global_instances.h
 *
 *  Created on: May 28, 2020
 *      Author: renanmoreira
 */

#ifndef INC_GLOBAL_INSTANCES_H_
#define INC_GLOBAL_INSTANCES_H_


extern TIM_HandleTypeDef htim2;
extern osMessageQueueId_t q_speed_messageHandle;
extern osThreadId_t t_main_taskHandle;
extern osEventFlagsId_t ECU_control_event_id;

#endif /* INC_GLOBAL_INSTANCES_H_ */
