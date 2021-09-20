/*
 * hook_handlers.c
 *
 *  Created on: Feb 1, 2021
 *      Author: renanmoreira
 */
#include "cmsis_os.h"
#include "stm32h7xx.h"

//void vApplicationMallocFailedHook(void) {
//	//erro de alocacao de memoria
//}

void brkpt() {
	;
}

void vAssertCalled( const char * pcFile,
                    unsigned long ulLine )
{
volatile unsigned long ul = 0;

    taskENTER_CRITICAL();
    {
        while( ul == 0 )
        {
            portNOP();
        }
    }
    taskEXIT_CRITICAL();
}
