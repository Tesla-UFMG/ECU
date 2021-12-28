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
typedef uint16_t GYRO_YAW_t;
typedef uint8_t INTERNAL_WHEEL_t;
typedef race_mode_t RACE_MODE_t;
typedef uint16_t THROTTLE_PERCENT_t;
typedef bool BRAKE_STATUS_t;
typedef bool THROTTLE_STATUS_t;
typedef modos SELECTED_MODE_t;

#define MOTOR_SPEEDS_DEFAULT_VALUE                                                                 \
    { 0, 0 }
#define WHEEL_SPEEDS_DEFAULT_VALUE                                                                 \
    { 0, 0, 0, 0 }
#define STEERING_WHEEL_DEFAULT_VALUE 0
#define GYRO_YAW_DEFAULT_VALUE 0
#define INTERNAL_WHEEL_DEFAULT_VALUE 0
#define RACE_MODE_DEFAULT_VALUE ENDURO
#define THROTTLE_PERCENT_DEFAULT_VALUE 0
#define BRAKE_STATUS_DEFAULT_VALUE false
#define THROTTLE_STATUS_DEFAULT_VALUE false
#define SELECTED_MODE_DEFAULT_VALUE                                                                \
    {}

typedef enum {
    MOTOR_SPEEDS,
    WHEEL_SPEEDS,
    STEERING_WHEEL,
    GYRO_YAW,
    INTERNAL_WHEEL,
    RACE_MODE,
    THROTTLE_PERCENT,
    BRAKE_STATUS,
    THROTTLE_STATUS,
    SELECTED_MODE,
    _global_vars_quan /* THIS MUST BE THE LAST ELEMENT OF THE ENUM, AS IT'S USED
                         A NUMBER-OF-ELEMENTS INDICATOR */
} global_vars_e;

void global_variables_init();

void get_global_var(global_vars_e type, void *message);
void set_global_var(global_vars_e type, void *message);

/*
 * Use the following macros to get/set the global variables without the need of
 * instantianting another intermediary auxiliary variable.
 * Keep in mind those are MACROS, not FUNCTIONS! This means that what's defined
 * as their "implementation body" is actually replaced as-is at the exact spot
 * where the invocation of the macros themselves happens. For example, the
 * following code: int main()
 * {
 *      int a;
 *      a = 1;
 *
 *      set_global_var_value(THROTTLE_PERCENT, 10);
 *
 *      set_global_var_value(THROTTLE_PERCENT, a);
 *
 *      THROTTLE_PERCENT_t thr = get_global_var_value(THROTTLE_PERCENT);
 * }
 *
 * would get replaced, during pre-processing stage of compilation, to:
 * int main()
 * {
 *      int a;
 *      a = 1;
 *
 *      {
 *          THROTTLE_PERCENT_t THROTTLE_PERCENT_var = 10;
 *          set_global_var(THROTTLE_PERCENT, &(THROTTLE_PERCENT_var));
 *      }
 *
 *      {
 *          THROTTLE_PERCENT_t THROTTLE_PERCENT_var = a;
 *          set_global_var(THROTTLE_PERCENT, &(THROTTLE_PERCENT_var));
 *      }
 *
 *      THROTTLE_PERCENT_t thr = ({
 *                                  THROTTLE_PERCENT_t THROTTLE_PERCENT_var;
 *                                  get_global_var(THROTTLE_PERCENT,
 * &THROTTLE_PERCENT_var); THROTTLE_PERCENT_var;
 *                               });
 * }
 *
 * This means the macros can't be used as 1:1 replacement for functions, as they
 * are simply rules for code substitution. Care must be taken as to where to
 * call the macros, as the substitution can lead to undefined behavior.
 */

/*
 * set_global_var_value(type, value)
 * \param[in]   type    the global variable, identified by 'global_vars_e' enum
 * \param[in]   value   value to be assigned to global variable. Must be
 * convertible to the global variables's defined type. \return void \example if
 * called like 'set_global_var_value(THROTTLE_PERCENT, 100)' – i.e.
 * type=THROTTLE_PERCENT and value=100 – would get expanded to:
 *
 *  {
 *    THROTTLE_PERCENT_t THROTTLE_PERCENT_var = 100;
 *    set_global_var(THROTTLE_PERCENT, &(THROTTLE_PERCENT_var));
 *  }
 *
 * The curly braces are there so the code gets executed inside an anonymous
 * temporary execution block, so it doesn't mess up with the scope where the
 * macro is invoked (by introducing a new variable whose lifetime is
 * uniquely-dependant on set_global_var call)
 */
#define set_global_var_value(type, value)                                                          \
    {                                                                                              \
        type##_t type##_var = value;                                                               \
        set_global_var(type, &(type##_var));                                                       \
    }

/*
 * get_global_var_value(type)
 * \param[in]   type    the global variable, identified by 'global_vars_e' enum
 *
 * \return the current value of the global variable, of type 'type_t' (e.g.
 * type=THROTTLE_PERCENT would yield THROTTLE_PERCENT_t) \example if called like
 * 'THROTTLE_PERCENT_t thr = get_global_var_value(THROTTLE_PERCENT)' – i.e.
 * type=THROTTLE_PERCENT – would get expanded to:
 *
 *      THROTTLE_PERCENT_t thr = ({
 *                                   THROTTLE_PERCENT_t THROTTLE_PERCENT_var;
 *                                   get_global_var(THROTTLE_PERCENT,
 * &THROTTLE_PERCENT_var); THROTTLE_PERCENT_var;
 *                                });
 *
 * The '({})' block is a Statement Expression, from GNU C. It allows
 * declarations and statements within an expression, being the last
 * expression-like statement the "return" value of the expression. This acts
 * like an anonymous execution block, avoid the pollution of the scope in which
 * the macro was invoked.
 */
#define get_global_var_value(type)                                                                 \
    ({                                                                                             \
        type##_t type##_var;                                                                       \
        get_global_var(type, &type##_var);                                                         \
        type##_var;                                                                                \
    })

#endif /* INC_CMSIS_EXTRA_GLOBAL_VARIABLES_HANDLER_H_ */
