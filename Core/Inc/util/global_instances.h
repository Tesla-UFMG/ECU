/*
 * global_instances.h
 *
 *  Created on: May 28, 2020
 *      Author: renanmoreira
 */

#ifndef INC_GLOBAL_INSTANCES_H_
#define INC_GLOBAL_INSTANCES_H_

#include "cmsis_os.h"
#include "stm32h7xx.h"

// PERIPHERALS
extern TIM_HandleTypeDef htim2;

// QUEUES
extern osMessageQueueId_t q_encoder_int_messageHandle;
extern osMessageQueueId_t q_torque_messageHandle;
extern osMessageQueueId_t q_datalog_messageHandle;
extern osMessageQueueId_t q_debug_leds_messageHandle;
extern osMessageQueueId_t q_rgb_led_messageHandle;
extern osMessageQueueId_t q_throttle_controlHandle;
extern osMessageQueueId_t q_encoder_speeds_messageHandle;
extern osMessageQueueId_t q_odometer_calc_save_messageHandle;

// TASKS
extern osThreadId_t t_main_taskHandle;
extern osThreadId_t t_seleciona_modoHandle;
extern osThreadId_t t_RTDHandle;
extern osThreadId_t t_pilot_resetHandle;
extern osThreadId_t t_inverter_comm_errorHandle;
extern osThreadId_t t_odometer_saveHandle;
extern osThreadId_t t_dynamic_control_choiceHandle;

// EVENT FLAGS
extern osEventFlagsId_t e_ECU_control_flagsHandle;

// SEMAPHORES

// MUTEXES
extern osMutexId_t m_state_parameter_mutexHandle;

// TIMERS
extern osTimerId_t tim_SU_F_errorHandle;
extern osTimerId_t tim_APPS_errorHandle;
;
extern osTimerId_t tim_inverter_BUS_OFF_errorHandle;
extern osTimerId_t tim_inverter_readyHandle;
extern osTimerId_t tim_inverter_can_transmit_errorHandle;

#endif /* INC_GLOBAL_INSTANCES_H_ */
