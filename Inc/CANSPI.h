#ifndef __CAN_SPI_H
#define	__CAN_SPI_H

#include "stm32f1xx_hal.h"
#include "stdbool.h"

typedef union {
  struct {
    uint8_t idType;
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
  } frame;
  uint8_t array[14];
} uCAN_MSG;

#define dSTANDARD_CAN_MSG_ID_2_0B 1
#define dEXTENDED_CAN_MSG_ID_2_0B 2

uCAN_MSG txMessage;
uCAN_MSG rxMessage;

bool CANSPI_Initialize(void);
void CANSPI_Sleep(void);
uint8_t CANSPI_Transmit(uint16_t id, uint8_t len, uint8_t* data);
uint8_t CANSPI_messagesInBuffer(void);
uint8_t CANSPI_isBussOff(void);
uint8_t CANSPI_isRxErrorPassive(void);
uint8_t CANSPI_isTxErrorPassive(void);
void CAN_SPI_EXTI_Handler();

#endif	/* __CAN_SPI_H */
