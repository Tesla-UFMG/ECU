/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CAN/inverter_can.h"
#include "CAN/general_can.h"
#include "util/initializers.h"
#include "sensors/APPS.h"
#include "sensors/encoder_speed.h"
#include "util/global_instances.h"
#include "util/main_task.h"
#include "leds/debug_leds_handler.h"
#include "leds/rgb_led_handler.h"
#include "util/CMSIS_extra/global_variables_handler.h"
#include "datalogging/speed.h"
#include "datalogging/odometer_save.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan2;

IWDG_HandleTypeDef hiwdg1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* Definitions for t_main_task */
osThreadId_t t_main_taskHandle;
const osThreadAttr_t t_main_task_attributes = {
  .name = "t_main_task",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for t_torque_parameters */
osThreadId_t t_torque_parametersHandle;
const osThreadAttr_t t_torque_parameters_attributes = {
  .name = "t_torque_parameters",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_datalogger */
osThreadId_t t_dataloggerHandle;
const osThreadAttr_t t_datalogger_attributes = {
  .name = "t_datalogger",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_APPS_read */
osThreadId_t t_APPS_readHandle;
const osThreadAttr_t t_APPS_read_attributes = {
  .name = "t_APPS_read",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_steering_read */
osThreadId_t t_steering_readHandle;
const osThreadAttr_t t_steering_read_attributes = {
  .name = "t_steering_read",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_encoder_speed_calc */
osThreadId_t t_encoder_speed_calcHandle;
const osThreadAttr_t t_encoder_speed_calc_attributes = {
  .name = "t_encoder_speed_calc",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_odometer_calc */
osThreadId_t t_odometer_calcHandle;
const osThreadAttr_t t_odometer_calc_attributes = {
  .name = "t_odometer_calc",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_torque_message */
osThreadId_t t_torque_messageHandle;
const osThreadAttr_t t_torque_message_attributes = {
  .name = "t_torque_message",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_torque_manager */
osThreadId_t t_torque_managerHandle;
const osThreadAttr_t t_torque_manager_attributes = {
  .name = "t_torque_manager",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_debug_leds */
osThreadId_t t_debug_ledsHandle;
const osThreadAttr_t t_debug_leds_attributes = {
  .name = "t_debug_leds",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_rgb_led */
osThreadId_t t_rgb_ledHandle;
const osThreadAttr_t t_rgb_led_attributes = {
  .name = "t_rgb_led",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_seleciona_modo */
osThreadId_t t_seleciona_modoHandle;
const osThreadAttr_t t_seleciona_modo_attributes = {
  .name = "t_seleciona_modo",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_RTD */
osThreadId_t t_RTDHandle;
const osThreadAttr_t t_RTD_attributes = {
  .name = "t_RTD",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_throttle_control */
osThreadId_t t_throttle_controlHandle;
const osThreadAttr_t t_throttle_control_attributes = {
  .name = "t_throttle_control",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_datalog_acquisition */
osThreadId_t t_datalog_acquisitionHandle;
const osThreadAttr_t t_datalog_acquisition_attributes = {
  .name = "t_datalog_acquisition",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_inverter_comm_error */
osThreadId_t t_inverter_comm_errorHandle;
const osThreadAttr_t t_inverter_comm_error_attributes = {
  .name = "t_inverter_comm_error",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_inverter_datalog */
osThreadId_t t_inverter_datalogHandle;
const osThreadAttr_t t_inverter_datalog_attributes = {
  .name = "t_inverter_datalog",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_pilot_reset */
osThreadId_t t_pilot_resetHandle;
const osThreadAttr_t t_pilot_reset_attributes = {
  .name = "t_pilot_reset",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_buttons_handler */
osThreadId_t t_buttons_handlerHandle;
const osThreadAttr_t t_buttons_handler_attributes = {
  .name = "t_buttons_handler",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_speed_datalog */
osThreadId_t t_speed_datalogHandle;
const osThreadAttr_t t_speed_datalog_attributes = {
  .name = "t_speed_datalog",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_odometer_save */
osThreadId_t t_odometer_saveHandle;
const osThreadAttr_t t_odometer_save_attributes = {
  .name = "t_odometer_save",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t_dynamic_controls_choice */
osThreadId_t t_dynamic_controls_choiceHandle;
const osThreadAttr_t t_dynamic_controls_choice_attributes = {
  .name = "t_dynamic_controls_choice",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for q_encoder_int_message */
osMessageQueueId_t q_encoder_int_messageHandle;
const osMessageQueueAttr_t q_encoder_int_message_attributes = {
  .name = "q_encoder_int_message"
};
/* Definitions for q_torque_message */
osMessageQueueId_t q_torque_messageHandle;
const osMessageQueueAttr_t q_torque_message_attributes = {
  .name = "q_torque_message"
};
/* Definitions for q_ref_torque_message */
osMessageQueueId_t q_ref_torque_messageHandle;
const osMessageQueueAttr_t q_ref_torque_message_attributes = {
  .name = "q_ref_torque_message"
};
/* Definitions for q_datalog_message */
osMessageQueueId_t q_datalog_messageHandle;
const osMessageQueueAttr_t q_datalog_message_attributes = {
  .name = "q_datalog_message"
};
/* Definitions for q_debug_leds_message */
osMessageQueueId_t q_debug_leds_messageHandle;
const osMessageQueueAttr_t q_debug_leds_message_attributes = {
  .name = "q_debug_leds_message"
};
/* Definitions for q_rgb_led_message */
osMessageQueueId_t q_rgb_led_messageHandle;
const osMessageQueueAttr_t q_rgb_led_message_attributes = {
  .name = "q_rgb_led_message"
};
/* Definitions for q_throttle_control */
osMessageQueueId_t q_throttle_controlHandle;
const osMessageQueueAttr_t q_throttle_control_attributes = {
  .name = "q_throttle_control"
};
/* Definitions for q_encoder_speeds_message */
osMessageQueueId_t q_encoder_speeds_messageHandle;
const osMessageQueueAttr_t q_encoder_speeds_message_attributes = {
  .name = "q_encoder_speeds_message"
};
/* Definitions for q_odometer_calc_save_message */
osMessageQueueId_t q_odometer_calc_save_messageHandle;
const osMessageQueueAttr_t q_odometer_calc_save_message_attributes = {
  .name = "q_odometer_calc_save_message"
};
/* Definitions for q_ids_can_inverter */
osMessageQueueId_t q_ids_can_inverterHandle;
const osMessageQueueAttr_t q_ids_can_inverter_attributes = {
  .name = "q_ids_can_inverter"
};
/* Definitions for tim_SU_F_error */
osTimerId_t tim_SU_F_errorHandle;
const osTimerAttr_t tim_SU_F_error_attributes = {
  .name = "tim_SU_F_error"
};
/* Definitions for tim_APPS_error */
osTimerId_t tim_APPS_errorHandle;
const osTimerAttr_t tim_APPS_error_attributes = {
  .name = "tim_APPS_error"
};
/* Definitions for tim_inverter_BUS_OFF_error */
osTimerId_t tim_inverter_BUS_OFF_errorHandle;
const osTimerAttr_t tim_inverter_BUS_OFF_error_attributes = {
  .name = "tim_inverter_BUS_OFF_error"
};
/* Definitions for tim_inverter_ready */
osTimerId_t tim_inverter_readyHandle;
const osTimerAttr_t tim_inverter_ready_attributes = {
  .name = "tim_inverter_ready"
};
/* Definitions for tim_inverter_can_transmit_error */
osTimerId_t tim_inverter_can_transmit_errorHandle;
const osTimerAttr_t tim_inverter_can_transmit_error_attributes = {
  .name = "tim_inverter_can_transmit_error"
};
/* Definitions for tim_left_inv_error */
osTimerId_t tim_left_inv_errorHandle;
const osTimerAttr_t tim_left_inv_error_attributes = {
  .name = "tim_left_inv_error"
};
/* Definitions for tim_right_inv_error */
osTimerId_t tim_right_inv_errorHandle;
const osTimerAttr_t tim_right_inv_error_attributes = {
  .name = "tim_right_inv_error"
};
/* Definitions for m_state_parameter_mutex */
osMutexId_t m_state_parameter_mutexHandle;
const osMutexAttr_t m_state_parameter_mutex_attributes = {
  .name = "m_state_parameter_mutex"
};
/* Definitions for e_ECU_control_flags */
osEventFlagsId_t e_ECU_control_flagsHandle;
const osEventFlagsAttr_t e_ECU_control_flags_attributes = {
  .name = "e_ECU_control_flags"
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_FDCAN1_Init(void);
static void MX_FDCAN2_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_IWDG1_Init(void);
void main_task(void *argument);
extern void torque_parameters(void *argument);
extern void datalogger(void *argument);
extern void APPS_read(void *argument);
extern void steering_read(void *argument);
extern void encoder_speed_calc(void *argument);
extern void odometer_calc(void *argument);
extern void torque_message(void *argument);
extern void torque_manager(void *argument);
extern void debug_leds(void *argument);
extern void rgb_led(void *argument);
extern void seleciona_modo(void *argument);
extern void RTD(void *argument);
extern void throttle_control(void *argument);
extern void datalog_acquisition(void *argument);
extern void inverter_comm_error(void *argument);
extern void inverter_datalog(void *argument);
extern void pilot_reset(void *argument);
extern void buttons_handler(void *argument);
extern void speed_datalog(void *argument);
extern void odometer_save(void *argument);
extern void dynamic_controls_choice(void *argument);
extern void errors_with_timer_callback(void *argument);
extern void inverter_BUS_OFF_error_callback(void *argument);
extern void inverter_ready_callback(void *argument);
extern void left_inv_error_callback(void *argument);
extern void right_inv_error_callback(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
//  init_NVIC_priorities();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_FDCAN1_Init();
  MX_FDCAN2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_IWDG1_Init();
  /* USER CODE BEGIN 2 */
  init_ECU();
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of m_state_parameter_mutex */
  m_state_parameter_mutexHandle = osMutexNew(&m_state_parameter_mutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of tim_SU_F_error */
  tim_SU_F_errorHandle = osTimerNew(errors_with_timer_callback, osTimerOnce, (void*) SU_F_ERROR_FLAG, &tim_SU_F_error_attributes);

  /* creation of tim_APPS_error */
  tim_APPS_errorHandle = osTimerNew(errors_with_timer_callback, osTimerOnce, (void*) APPS_ERROR_FLAG, &tim_APPS_error_attributes);

  /* creation of tim_inverter_BUS_OFF_error */
  tim_inverter_BUS_OFF_errorHandle = osTimerNew(inverter_BUS_OFF_error_callback, osTimerOnce, NULL, &tim_inverter_BUS_OFF_error_attributes);

  /* creation of tim_inverter_ready */
  tim_inverter_readyHandle = osTimerNew(inverter_ready_callback, osTimerOnce, NULL, &tim_inverter_ready_attributes);

  /* creation of tim_inverter_can_transmit_error */
  tim_inverter_can_transmit_errorHandle = osTimerNew(errors_with_timer_callback, osTimerOnce, (void*) INVERTER_CAN_TRANSMIT_ERROR_FLAG, &tim_inverter_can_transmit_error_attributes);

  /* creation of tim_left_inv_error */
  tim_left_inv_errorHandle = osTimerNew(left_inv_error_callback, osTimerPeriodic, (void*) LEFT_INVERTER_COMM_ERROR_FLAG, &tim_left_inv_error_attributes);

  /* creation of tim_right_inv_error */
  tim_right_inv_errorHandle = osTimerNew(right_inv_error_callback, osTimerPeriodic, (void*) RIGHT_INVERTER_COMM_ERROR_FLAG, &tim_right_inv_error_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of q_encoder_int_message */
  q_encoder_int_messageHandle = osMessageQueueNew (16, sizeof(encoder_int_message_t), &q_encoder_int_message_attributes);

  /* creation of q_torque_message */
  q_torque_messageHandle = osMessageQueueNew (16, sizeof(torque_message_t), &q_torque_message_attributes);

  /* creation of q_ref_torque_message */
  q_ref_torque_messageHandle = osMessageQueueNew (16, sizeof(ref_torque_t), &q_ref_torque_message_attributes);

  /* creation of q_datalog_message */
  q_datalog_messageHandle = osMessageQueueNew (128, sizeof(datalog_message_t), &q_datalog_message_attributes);

  /* creation of q_debug_leds_message */
  q_debug_leds_messageHandle = osMessageQueueNew (16, sizeof(debug_led_message_t), &q_debug_leds_message_attributes);

  /* creation of q_rgb_led_message */
  q_rgb_led_messageHandle = osMessageQueueNew (16, sizeof(rgb_led_message_t), &q_rgb_led_message_attributes);

  /* creation of q_throttle_control */
  q_throttle_controlHandle = osMessageQueueNew (16, sizeof(uint16_t), &q_throttle_control_attributes);

  /* creation of q_encoder_speeds_message */
  q_encoder_speeds_messageHandle = osMessageQueueNew (1, sizeof(encoder_speeds_message_t), &q_encoder_speeds_message_attributes);

  /* creation of q_odometer_calc_save_message */
  q_odometer_calc_save_messageHandle = osMessageQueueNew (1, sizeof(odometer_message_t), &q_odometer_calc_save_message_attributes);

  /* creation of q_ids_can_inverter */
  q_ids_can_inverterHandle = osMessageQueueNew (32, sizeof(uint32_t), &q_ids_can_inverter_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of t_main_task */
  t_main_taskHandle = osThreadNew(main_task, NULL, &t_main_task_attributes);

  /* creation of t_torque_parameters */
  t_torque_parametersHandle = osThreadNew(torque_parameters, NULL, &t_torque_parameters_attributes);

  /* creation of t_datalogger */
  t_dataloggerHandle = osThreadNew(datalogger, NULL, &t_datalogger_attributes);

  /* creation of t_APPS_read */
  t_APPS_readHandle = osThreadNew(APPS_read, NULL, &t_APPS_read_attributes);

  /* creation of t_steering_read */
  t_steering_readHandle = osThreadNew(steering_read, NULL, &t_steering_read_attributes);

  /* creation of t_encoder_speed_calc */
  t_encoder_speed_calcHandle = osThreadNew(encoder_speed_calc, NULL, &t_encoder_speed_calc_attributes);

  /* creation of t_odometer_calc */
  t_odometer_calcHandle = osThreadNew(odometer_calc, NULL, &t_odometer_calc_attributes);

  /* creation of t_torque_message */
  t_torque_messageHandle = osThreadNew(torque_message, NULL, &t_torque_message_attributes);

  /* creation of t_torque_manager */
  t_torque_managerHandle = osThreadNew(torque_manager, NULL, &t_torque_manager_attributes);

  /* creation of t_debug_leds */
  t_debug_ledsHandle = osThreadNew(debug_leds, NULL, &t_debug_leds_attributes);

  /* creation of t_rgb_led */
  t_rgb_ledHandle = osThreadNew(rgb_led, NULL, &t_rgb_led_attributes);

  /* creation of t_seleciona_modo */
  t_seleciona_modoHandle = osThreadNew(seleciona_modo, NULL, &t_seleciona_modo_attributes);

  /* creation of t_RTD */
  t_RTDHandle = osThreadNew(RTD, NULL, &t_RTD_attributes);

  /* creation of t_throttle_control */
  t_throttle_controlHandle = osThreadNew(throttle_control, NULL, &t_throttle_control_attributes);

  /* creation of t_datalog_acquisition */
  t_datalog_acquisitionHandle = osThreadNew(datalog_acquisition, NULL, &t_datalog_acquisition_attributes);

  /* creation of t_inverter_comm_error */
  t_inverter_comm_errorHandle = osThreadNew(inverter_comm_error, NULL, &t_inverter_comm_error_attributes);

  /* creation of t_inverter_datalog */
  t_inverter_datalogHandle = osThreadNew(inverter_datalog, NULL, &t_inverter_datalog_attributes);

  /* creation of t_pilot_reset */
  t_pilot_resetHandle = osThreadNew(pilot_reset, NULL, &t_pilot_reset_attributes);

  /* creation of t_buttons_handler */
  t_buttons_handlerHandle = osThreadNew(buttons_handler, NULL, &t_buttons_handler_attributes);

  /* creation of t_speed_datalog */
  t_speed_datalogHandle = osThreadNew(speed_datalog, NULL, &t_speed_datalog_attributes);

  /* creation of t_odometer_save */
  t_odometer_saveHandle = osThreadNew(odometer_save, NULL, &t_odometer_save_attributes);

  /* creation of t_dynamic_controls_choice */
  t_dynamic_controls_choiceHandle = osThreadNew(dynamic_controls_choice, NULL, &t_dynamic_controls_choice_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of e_ECU_control_flags */
  e_ECU_control_flagsHandle = osEventFlagsNew(&e_ECU_control_flags_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B_OPT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 6;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_19;
  sConfig.Rank = ADC_REGULAR_RANK_6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief FDCAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = DISABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 5;
  hfdcan1.Init.NominalSyncJumpWidth = 4;
  hfdcan1.Init.NominalTimeSeg1 = 27;
  hfdcan1.Init.NominalTimeSeg2 = 4;
  hfdcan1.Init.DataPrescaler = 5;
  hfdcan1.Init.DataSyncJumpWidth = 4;
  hfdcan1.Init.DataTimeSeg1 = 27;
  hfdcan1.Init.DataTimeSeg2 = 4;
  hfdcan1.Init.MessageRAMOffset = 0;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.RxFifo0ElmtsNbr = 32;
  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxFifo1ElmtsNbr = 32;
  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxBuffersNbr = 32;
  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.TxEventsNbr = 32;
  hfdcan1.Init.TxBuffersNbr = 32;
  hfdcan1.Init.TxFifoQueueElmtsNbr = 32;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */

  /* USER CODE END FDCAN1_Init 2 */

}

/**
  * @brief FDCAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN2_Init(void)
{

  /* USER CODE BEGIN FDCAN2_Init 0 */

  /* USER CODE END FDCAN2_Init 0 */

  /* USER CODE BEGIN FDCAN2_Init 1 */

  /* USER CODE END FDCAN2_Init 1 */
  hfdcan2.Instance = FDCAN2;
  hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan2.Init.AutoRetransmission = DISABLE;
  hfdcan2.Init.TransmitPause = DISABLE;
  hfdcan2.Init.ProtocolException = DISABLE;
  hfdcan2.Init.NominalPrescaler = 1;
  hfdcan2.Init.NominalSyncJumpWidth = 2;
  hfdcan2.Init.NominalTimeSeg1 = 13;
  hfdcan2.Init.NominalTimeSeg2 = 2;
  hfdcan2.Init.DataPrescaler = 1;
  hfdcan2.Init.DataSyncJumpWidth = 2;
  hfdcan2.Init.DataTimeSeg1 = 13;
  hfdcan2.Init.DataTimeSeg2 = 2;
  hfdcan2.Init.MessageRAMOffset = 1280;
  hfdcan2.Init.StdFiltersNbr = 0;
  hfdcan2.Init.ExtFiltersNbr = 0;
  hfdcan2.Init.RxFifo0ElmtsNbr = 32;
  hfdcan2.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.RxFifo1ElmtsNbr = 32;
  hfdcan2.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.RxBuffersNbr = 32;
  hfdcan2.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.TxEventsNbr = 32;
  hfdcan2.Init.TxBuffersNbr = 32;
  hfdcan2.Init.TxFifoQueueElmtsNbr = 32;
  hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan2.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN2_Init 2 */

  /* USER CODE END FDCAN2_Init 2 */

}

/**
  * @brief IWDG1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG1_Init(void)
{

  /* USER CODE BEGIN IWDG1_Init 0 */

  /* USER CODE END IWDG1_Init 0 */

  /* USER CODE BEGIN IWDG1_Init 1 */

  /* USER CODE END IWDG1_Init 1 */
  hiwdg1.Instance = IWDG1;
  hiwdg1.Init.Prescaler = IWDG_PRESCALER_8;
  hiwdg1.Init.Window = 4095;
  hiwdg1.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG1_Init 2 */
  __HAL_DBGMCU_FREEZE_IWDG1() ;
  /* USER CODE END IWDG1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 1799;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 4000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFFFFFF;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, C_LED_DEBUG1_Pin|C_LED_DEBUG3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(C_LED_DEBUG2_GPIO_Port, C_LED_DEBUG2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, C_LED_BLUE_Pin|C_LED_GREEN_Pin|C_LED_RED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(C_RTDS_GPIO_Port, C_RTDS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : C_LED_DEBUG1_Pin C_LED_DEBUG2_Pin C_LED_DEBUG3_Pin */
  GPIO_InitStruct.Pin = C_LED_DEBUG1_Pin|C_LED_DEBUG2_Pin|C_LED_DEBUG3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : S_VEL1_Pin S_VEL2_Pin S_VEL3_Pin S_VEL4_Pin */
  GPIO_InitStruct.Pin = S_VEL1_Pin|S_VEL2_Pin|S_VEL3_Pin|S_VEL4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : C_LED_BLUE_Pin C_LED_GREEN_Pin C_LED_RED_Pin */
  GPIO_InitStruct.Pin = C_LED_BLUE_Pin|C_LED_GREEN_Pin|C_LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : B_MODE_Pin B_DYNAMICS_CONTROLS_Pin B_RTD_Pin */
  GPIO_InitStruct.Pin = B_MODE_Pin|B_DYNAMICS_CONTROLS_Pin|B_RTD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : C_RTDS_Pin */
  GPIO_InitStruct.Pin = C_RTDS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(C_RTDS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_main_task */
/**
  * @brief  Function implementing the t_main_task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_main_task */
__weak void main_task(void *argument)
{
  /* USER CODE BEGIN 5 */
  UNUSED(argument);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM3 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM3) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
