/*
 * global_variables_handler.h
 *
 *  Created on: 15 de set de 2021
 *      Author: renanmoreira
 */

#ifndef INC_CMSIS_EXTRA_GLOBAL_VARIABLES_HANDLER_H_
#define INC_CMSIS_EXTRA_GLOBAL_VARIABLES_HANDLER_H_

#include "global_definitions.h"
#include <stdint.h>

typedef struct {
    uint16_t speed[2];
} MOTOR_SPEEDS_t;
typedef struct {
    float speed[4];
} WHEEL_SPEEDS_t;
typedef uint16_t STEERING_WHEEL_t;
typedef uint8_t INTERNAL_WHEEL_t;
typedef race_mode_t RACE_MODE_t;
typedef uint16_t THROTTLE_PERCENT_t;
typedef bool BRAKE_STATUS_t;
typedef bool THROTTLE_STATUS_t;
typedef modos SELECTED_MODE_t;

#define MOTOR_SPEEDS_DEFAULT_VALUE                                             \
    { 0, 0 }
#define WHEEL_SPEEDS_DEFAULT_VALUE                                             \
    { 0, 0, 0, 0 }
#define STEERING_WHEEL_DEFAULT_VALUE 0
#define INTERNAL_WHEEL_DEFAULT_VALUE 0
#define RACE_MODE_DEFAULT_VALUE ENDURO
#define THROTTLE_PERCENT_DEFAULT_VALUE 0
#define BRAKE_STATUS_DEFAULT_VALUE false
#define THROTTLE_STATUS_DEFAULT_VALUE false
#define SELECTED_MODE_DEFAULT_VALUE                                            \
    {}

typedef enum {
    MOTOR_SPEEDS,
    WHEEL_SPEEDS,
    STEERING_WHEEL,
    INTERNAL_WHEEL,
    RACE_MODE,
    THROTTLE_PERCENT,
    BRAKE_STATUS,
    THROTTLE_STATUS,
    SELECTED_MODE
} global_vars_message_e;

#define GLOBAL_VARS_QUAN 9

void global_variables_init();

void get_global_var(global_vars_message_e type, void *message);
void set_global_var(global_vars_message_e type, void *message);

#define set_global_var_value(type, value)                                      \
    {                                                                          \
        type##_t type##_var = value;                                           \
        set_global_var(type, &(type##_var));                                   \
    }

#define get_global_var_value(type)                                             \
    ({                                                                         \
        type##_t type##_var;                                                   \
        get_global_var(type, &type##_var);                                     \
        type##_var;                                                            \
    })

#endif /* INC_CMSIS_EXTRA_GLOBAL_VARIABLES_HANDLER_H_ */
