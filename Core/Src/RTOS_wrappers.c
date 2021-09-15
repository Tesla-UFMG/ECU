/*
 * RTOS_wrappers.c
 *
 *  Created on: 14 de set de 2021
 *      Author: Felipe Telles
 */

#include "RTOS_wrappers.h"
#include "cmsis_os.h"
#include "global_definitions.h"
#include "global_instances.h"

void wait_forever_for_RTD() {
    osEventFlagsWait(ECU_control_event_id, RTD_FLAG, osFlagsNoClear, osWaitForever);
}


