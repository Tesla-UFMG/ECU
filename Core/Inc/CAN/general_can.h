/*
 *      Communication via CAN - general_can.h
 *
 *      Created on: JUN 13, 2023
 *      Author: Gabriel Luiz
 *      Contact: (31) 97136-4334 || gabrielluiz.eletro@gmail.com
 */

#ifndef CAN_LOG_H_
#define CAN_LOG_H_

#define CHIP_STM32H7

//#define CHIP_DEBUG

#ifdef CHIP_DEBUG
#define FDCAN_MODO FDCAN_MODE_INTERNAL_LOOPBACK
#else
#define FDCAN_MODO FDCAN_MODE_NORMAL
#endif

#ifdef CHIP_STM32H7
#include "stm32h7xx.h"
#endif

#ifdef CHIP_STM32G0
#include "stm32g0xx_hal.h"
#endif

#ifdef CHIP_STM32G4
#include "stm32g4xx_hal.h"
#endif

#define CAN_IDS_NUMBER 500

/* Used to register a CAN buffer */
typedef enum
{
	FDCAN_INT,
	FDCAN_FLOAT,
	FDCAN_DOUBLE,
	FDCAN_FREE
} Data_type_t;

typedef enum
{
	FDCAN_OK,
	FDCAN_ERROR,
	FDCAN_TIMEOUT,
	FDCAN_RESET
} FDCAN_StatusTypedef;

typedef struct
{
	uint8_t Data_buf[CAN_IDS_NUMBER][8];
	Data_type_t Type_buf[CAN_IDS_NUMBER];
} CAN_Buffer_t;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CAN RECEIVE CALLBACK  --------------------------------------------------------------------------------*/
/**
 * @brief  Function called when a message is detected on the CAN bus
 * @param  hfdcan: CAN handle || normally "hfdcan1"
 * @param  RxFifo0ITs: FIFO where the message was detected
 * @retval ***NONE***
 */
//__weak void CAN_general_receive_callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- RECEIVED MESSAGE HANDLING -----------------------------------------------------------------------------*/
/**
 * @brief  Function for handling received messages
 * @param  hRxFDCAN: Handler with received frame information
 * @param  Buffer: Buffer with message data and information
 * @retval Function execution status
 */
FDCAN_StatusTypedef CAN_Stream_ReceiveCallback(FDCAN_RxHeaderTypeDef *hRxFDCAN, uint8_t *Buffer);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CAN CONFIGURATION PARAMETERS --------------------------------------------------------------*/
/**
 * @brief  Configures the CAN, overwriting .IOC settings
 * @param  ***NONE***
 * @retval Function execution status
 */
FDCAN_StatusTypedef CAN_Configure_Init(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- STORAGE BUFFERS CLEANUP --------------------------------------------------------------*/
/**
 * @brief  Initializes CAN message storage buffers
 * @param  ***NONE***
 * @retval ***NONE***
 */
void CAN_Clean_Buffers(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- FDCAN COMMUNICATION INITIALIZATION --------------------------------------------------------------*/
/**
 * @brief  Initializes FDCAN communication
 * @param  ***NONE***
 * @retval Function execution status
 */
FDCAN_StatusTypedef initialize_general_CAN(FDCAN_HandleTypeDef* can_ref);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- INTEGER VALUE STORAGE--------------------------------------------------------------*/
/**
 * @brief  Function for storing integer values, including type information
 * @param  Identifier: Message identifier
 * @param  Buffer: Pointer to buffer containing data and storage information
 * @retval ***NONE***
 */
void CAN_Storage_INT(uint16_t Identifier, uint8_t *Buffer);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- FLOAT VALUE STORAGE --------------------------------------------------------------*/
/**
 * @brief  Function for storing float values, including type information
 * @param  Identifier: Message identifier
 * @param  Buffer: Pointer to buffer containing data and storage information
 * @retval ***NONE***
 */
void CAN_Storage_FLOAT(uint16_t Identifier, uint8_t *Buffer);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- DOUBLE VALUE STORAGE  --------------------------------------------------------------*/
/**
 * @brief  Function for storing double values, including type information
 * @param  Identifier: Message identifier
 * @param  Buffer: Pointer to buffer containing data and storage information
 * @retval ***NONE***
 */
void CAN_Storage_DOUBLE(uint16_t Identifier, uint8_t *Buffer);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- ACCESS TO STORED INT VALUES --------------------------------------------------------------*/
/**
 * @brief  Function to access int values stored in memory
 * @param  Identifier: Message identifier
 * @retval Stored int value, returns "FDCAN_ERROR" if value isn't an int
 */
int32_t CAN_Get_value(uint16_t Identifier);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- ACCESS TO STORED FLOAT VALUES  --------------------------------------------------------------*/
/**
 * @brief  Function to access float values stored in memory
 * @param  Identifier: Message identifier
 * @retval Stored float value, returns "FDCAN_ERROR" if value isn't a float
 */
float CAN_Get_value_FLOAT(uint16_t Identifier);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- ACCESS TO STORED DOUBLE VALUES  --------------------------------------------------------------*/
/**
 * @brief  Function to access double values stored in memory
 * @param  Identifier: Message identifier
 * @retval Stored double value, returns "FDCAN_ERROR" if value isn't a double
 */
double CAN_Get_value_DOUBLE(uint16_t Identifier);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*----  ADDING MESSAGES TO SEND BUFFER--------------------------------------------------------------*/
/**
 * @brief  Function to add a message to the send buffer
 * @param  Identifier: Message identifier
 * @param  Buffer: Data buffer to send
 * @param  Size: Number of bytes to send
 * @retval Function execution status
 */
FDCAN_StatusTypedef CAN_TxData(uint16_t Identifier, uint64_t Buffer, uint8_t Size);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- INT PROCESSING AND SENDING --------------------------------------------------------------*/
/**
 * @brief  Function for processing and sending int values via bus
 * @param  Identifier: Message identifier
 * @param  Value: Int value to send via bus
 * @retval Function execution status
 */
FDCAN_StatusTypedef CAN_Send(uint16_t Identifier, int32_t Value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- FLOAT PROCESSING AND SENDING --------------------------------------------------------------*/
/**
 * @brief  Function for processing and sending float values via bus
 * @param  Identifier: Message identifier
 * @param  Data: Float value to send via bus
 * @retval Function execution status
 */
FDCAN_StatusTypedef CAN_Send_Float(uint16_t Identifier, float Data);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- DOUBLE PROCESSING AND SENDING --------------------------------------------------------------*/
/**
 * @brief  Function for processing and sending double values via bus
 * @param  Identifier: Message identifier
 * @param  Data: Double value to send via bus
 * @retval Function execution status
 */
FDCAN_StatusTypedef CAN_Send_Double(uint16_t Identifier, double Data);

#endif /* CAN_LOG_H_ */