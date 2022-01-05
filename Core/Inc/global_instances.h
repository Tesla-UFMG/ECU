/*
 * global_instances.h
 *
 *  Created on: May 28, 2020
 *      Author: renanmoreira
 */

#ifndef INC_GLOBAL_INSTANCES_H_
#define INC_GLOBAL_INSTANCES_H_

#include "stm32h7xx.h"
#include "cmsis_os.h"

//PERIFERICOS
extern TIM_HandleTypeDef htim2;

//QUEUES
extern osMessageQueueId_t q_speed_messageHandle;
extern osMessageQueueId_t q_torque_messageHandle;
extern osMessageQueueId_t q_datalog_messageHandle;
extern osMessageQueueId_t q_debugleds_messageHandle;
extern osMessageQueueId_t q_rgb_led_messageHandle;
extern osMessageQueueId_t q_throttle_controlHandle;

//TASKS
extern osThreadId_t t_main_taskHandle;
extern osThreadId_t t_seleciona_modoHandle;
extern osThreadId_t t_RTDHandle;
extern osThreadId_t t_inverter_no_comm_errorHandle;

//EVENT FLAGS
extern osEventFlagsId_t ECU_control_event_id;

//SEMAPHORES

//MUTEXES
extern osMutexId_t m_state_parameter_mutexHandle;

//TIMERS
extern osTimerId_t tim_SU_F_errorHandle;
extern osTimerId_t tim_APPS_errorHandle;;
extern osTimerId_t tim_inverter_BUS_OFF_errorHandle;
extern osTimerId_t tim_inverter_readyHandle;

#endif /* INC_GLOBAL_INSTANCES_H_ */
