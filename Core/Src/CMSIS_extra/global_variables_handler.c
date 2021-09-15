/*
 * global_variables_handler.c
 *
 *  Created on: 15 de set de 2021
 *      Author: renanmoreira
 */

#include "CMSIS_extra/global_variables_handler.h"

#include "CMSIS_extra/cmsis_extra.h"
#include "cmsis_os2.h"

typedef struct {
    osMessageQueueId_t id;
    osMessageQueueAttr_t attr;
} global_variable_message_queue_data_t;

// create local storage of queue infos
static global_variable_message_queue_data_t queues_info[GLOBAL_VARS_QUAN];

#define INSTANTIATE_GLOBAL_VAR_QUEUE(var_name)                                 \
    global_variable_message_queue_data_t *var_name##_info =                    \
        queues_info + var_name;                                                \
    var_name##_info->attr = (osMessageQueueAttr_t){.name = "q_" #var_name};    \
    var_name##_info->id = osMessageQueueNew(/*size=*/1, sizeof(var_name##_t),  \
                                            &var_name##_info->attr);           \
    var_name##_t var_name##_msg = (var_name##_t)var_name##_DEFAULT_VALUE;      \
    osMessageQueuePut(var_name##_info->id, &var_name##_msg, 0, 0);

void global_variables_init() {

    // instantiating single-element message queues for each global variable
    INSTANTIATE_GLOBAL_VAR_QUEUE(MOTOR_SPEEDS);
    INSTANTIATE_GLOBAL_VAR_QUEUE(WHEEL_SPEEDS);
    INSTANTIATE_GLOBAL_VAR_QUEUE(STEERING_WHEEL);
    INSTANTIATE_GLOBAL_VAR_QUEUE(INTERNAL_WHEEL);
    INSTANTIATE_GLOBAL_VAR_QUEUE(RACE_MODE);
    INSTANTIATE_GLOBAL_VAR_QUEUE(THROTTLE_PERCENT);
    INSTANTIATE_GLOBAL_VAR_QUEUE(BRAKE_STATUS);
    INSTANTIATE_GLOBAL_VAR_QUEUE(THROTTLE_STATUS);
    INSTANTIATE_GLOBAL_VAR_QUEUE(SELECTED_MODE);
}

void get_global_var(global_vars_message_e type, void *message) {
    osMessageQueuePeek(queues_info[type].id, message, 0, 0);
}

void set_global_var(global_vars_message_e type, void *message) {
    osMessageQueuePutOverwrite(queues_info[type].id, message, 0);
}
