/*
 * global_variables_handler.c
 *
 *  Created on: 15 de set de 2021
 *      Author: renanmoreira
 */

#include "util/CMSIS_extra/global_variables_handler.h"

#include "cmsis_os2.h"
#include "util/CMSIS_extra/cmsis_extra.h"

typedef struct {
    osMessageQueueId_t id;
    osMessageQueueAttr_t attr;
} global_variable_message_queue_data_t;

// create local storage of queue infos
static global_variable_message_queue_data_t queues_info[_global_vars_quan];

/*
 * This macro creates a Message Queue of size 1, based on the name provided by
 * var_name parameter. The Message Queue Id is then stored in the array
 * queues_info, at the position identified by the integer value from the enum
 * named var_name (e.g. var_name=THROTTLE_PERCENT would translate to enum
 * THROTTLE_PERCENT, defined in global_vars_e). The call
 * 'INSTANTIATE_GLOBAL_VAR_QUEUE(THROTTLE_PERCENT)' would get expanded to:
 *
 * global_variable_message_queue_data_t *THROTTLE_PERCENT_info = queues_info + var_name;
 * THROTTLE_PERCENT_info->attr = (osMessageQueueAttr_t){.name = "q_THROTTLE_PERCENT"};
 * THROTTLE_PERCENT_info->id = osMessageQueueNew(1, sizeof(THROTTLE_PERCENT_t),
 * &THROTTLE_PERCENT_info->attr); THROTTLE_PERCENT_t THROTTLE_PERCENT_msg =
 * (THROTTLE_PERCENT_t)THROTTLE_PERCENT_DEFAULT_VALUE;
 * osMessageQueuePut(THROTTLE_PERCENT_info->id, &THROTTLE_PERCENT_msg, 0, 0);
 */
#define INSTANTIATE_GLOBAL_VAR_QUEUE(var_name)                                           \
    /* get a pointer to the queue_data struct intance at position var_name*/             \
    global_variable_message_queue_data_t* var_name##_info = queues_info + (var_name);    \
    /* defines queue name as q_var_name */                                               \
    var_name##_info->attr = (osMessageQueueAttr_t){.name = "q_" #var_name};              \
    /* create queue of size 1, storing the queue id in the pointer created               \
     * previously */                                                                     \
    var_name##_info->id =                                                                \
        osMessageQueueNew(/*size=*/1, sizeof(var_name##_t), &var_name##_info->attr);     \
    /* fetch var default value defined in the header file, storing it in a               \
     * temp var*/                                                                        \
    var_name##_t var_name##_msg = (var_name##_t)var_name##_DEFAULT_VALUE;                \
    /* place this value in the queue */                                                  \
    osMessageQueuePut(var_name##_info->id, &var_name##_msg, 0, 0);

void init_global_variables() {

    // instantiating single-element message queues for each global variable
    INSTANTIATE_GLOBAL_VAR_QUEUE(SPEEDS);
    INSTANTIATE_GLOBAL_VAR_QUEUE(FRONT_AVG_SPEED);
    INSTANTIATE_GLOBAL_VAR_QUEUE(REAR_AVG_SPEED);
    INSTANTIATE_GLOBAL_VAR_QUEUE(STEERING_WHEEL);
    INSTANTIATE_GLOBAL_VAR_QUEUE(GYRO_YAW);
    INSTANTIATE_GLOBAL_VAR_QUEUE(INTERNAL_WHEEL);
    INSTANTIATE_GLOBAL_VAR_QUEUE(RACE_MODE);
    INSTANTIATE_GLOBAL_VAR_QUEUE(THROTTLE_PERCENT);
    INSTANTIATE_GLOBAL_VAR_QUEUE(BRAKE_STATUS);
    INSTANTIATE_GLOBAL_VAR_QUEUE(THROTTLE_STATUS);
    INSTANTIATE_GLOBAL_VAR_QUEUE(SELECTED_MODE);
    INSTANTIATE_GLOBAL_VAR_QUEUE(SOMADOR_TASK);
}

void get_global_var(global_vars_e type, void* message) {
    osMessageQueuePeek(queues_info[type].id, message, 0, 0);
}

void set_global_var(global_vars_e type, void* message) {
    osMessageQueuePutOverwrite(queues_info[type].id, message, 0);
}
