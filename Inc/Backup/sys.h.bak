/*
 * sys.h
 *
 *  Created on: 10 Aug 2018
 *      Author: r0t
 */

#ifndef __SYS_H_
#define __SYS_H_

#include "main.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "wwdg.h"
#include "gpio.h"
#include "CANSPI.h"




#define __DLC__ 8

#define delay_ms(x) HAL_Delay(x)

void SystemClock_Config(void);



/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */



#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif


#endif /* __SYS_H_ */
