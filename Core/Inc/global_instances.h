/*
 * global_instances.h
 *
 *  Created on: May 28, 2020
 *      Author: renanmoreira
 */

#ifndef INC_GLOBAL_INSTANCES_H_
#define INC_GLOBAL_INSTANCES_H_

//PERIFERICOS
extern TIM_HandleTypeDef htim2;

//QUEUES
extern osMessageQueueId_t q_speed_messageHandle;
extern osMessageQueueId_t q_torque_messageHandle;

//TASKS
extern osThreadId_t t_main_taskHandle;

//EVENT FLAGS
extern osEventFlagsId_t ECU_control_event_id;

//MUTEXES
extern osMutexId_t m_state_parameter_mutexHandle;

#endif /* INC_GLOBAL_INSTANCES_H_ */
