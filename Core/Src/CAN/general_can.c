/*
 *      Communication via FDCAN - general_can.c
 *
 *      Created on: NOV 01, 2023
 *      Author: Gabriel Luiz
 *      Contact: (31) 97136-4334 || gabrielluiz.eletro@gmail.com
 * 
 * 		- Useful links -
 *
 *      DATASHEET: https://www.st.com/content/ccc/resource/training/technical/product_training/group0/35/ed/76/ef/91/30/44/f7/STM32H7-Peripheral-Flexible_Datarate_Controller_Area_Network_FDCAN/files/STM32H7-Peripheral-Flexible_Datarate_Controller_Area_Network_FDCAN.pdf/_jcr_content/translations/en.STM32H7-Peripheral-Flexible_Datarate_Controller_Area_Network_FDCAN.pdf
 *      FDCAN normal DOCUMENT: https://controllerstech.com/fdcan-normal-mode-stm32/
 *      FDCAN normal mode VÍDEO: https://www.youtube.com/watch?v=sY1ie-CnOR0&t=7s
 */

 #include <stdlib.h>
 #include <string.h>
 #include <math.h>

 #include "cmsis_os2.h"
 
 #include "CAN/general_can.h"
 #include "CAN/CAN_IDs.h"
 #include "CAN/CAN_handler.h"
 #include "CAN/general_can_data_manager.h"
 #include "leds/debug_leds_handler.h"
 #include "util/global_definitions.h"
 #include "util/global_instances.h"
 #include "util/util.h"
 
 /* External variables --------------------------------------------------------*/
 /* USER CODE BEGIN EV */
 
 extern FDCAN_HandleTypeDef hfdcan2; /* External CAN configuration variable */
 
 /* Private define ------------------------------------------------------------*/
 /* USER CODE BEGIN PD */
 
 static void CAN_general_receive_callback(FDCAN_HandleTypeDef* /*hfdcan*/,
         uint32_t /*RxFifo0ITs*/);
 static void CAN_general_error_callback(FDCAN_HandleTypeDef* /*hfdcan*/,
         uint32_t /*ErrorStatusITs*/);
 
 #define TIME_TO_BREAK 10 /* Maximum time for sending a CAN message (ms) */
 
 #define INT_SIZE 4
 
 #define FLOAT_SIZE 5
 
 #define DOUBLE_SIZE 8
 
 /* USER CODE END PD */
 
 /* External functions ------------------------------------------------------------*/
 /* USER CODE BEGIN EF */
 
 extern void Error_Handler(); /* Function used for error handling */
 
 /* USER CODE END EF */
 
 /* Private variables --------------------------------------------------------*/
 /* USER CODE BEGIN PV */
 
 FDCAN_HandleTypeDef *hFDCAN = &hfdcan2; /* CAN configuration handler */
 
 CAN_Buffer_t CAN_stream; /* Array for storing all CAN data */
 
 static FDCAN_TxHeaderTypeDef TxHeader; /* Temporary struct for
storing information and data for CAN transmission – Does not include the data */
 
 static FDCAN_RxHeaderTypeDef RxHeader; /* Temporary struct for 
storing information received via CAN – Does not include the data */
 
 static uint8_t RxData[8]; /* Temporary array for storing data received via CAN */
 
 uint16_t CONT_Telemetry;
 uint16_t CONT_Datalogger;
 uint16_t CONT_BMS;
 uint16_t CONT_ECU;
 
 uint8_t CONT_DEBUG = 0;
 
 FDCAN_StatusTypedef FDCAN_Status = FDCAN_RESET;
 /* USER CODE END PV */
 
 /* Private functions ------------------------------------------------------------*/
 /* USER CODE BEGIN PF */
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CAN RECEIVE CALLBACK --------------------------------------------------------------------------------*/
/**
 * @brief  Function called when a message is detected on the CAN bus
 * @param  hfdcan: CAN handle || normally "hfdcan1"
 * @param  RxFifo0ITs: FIFO where the message was detected
 * @retval ***NONE***
 */

  void CAN_general_receive_callback(FDCAN_HandleTypeDef *hfdcan,
         uint32_t RxFifo0ITs) {
 
//	/* Toggles LED 2 if there's something to receive via CAN */
#ifdef CHIP_LED_DEBUG
     HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
 #endif //CHIP_LED_DEBUG
     if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
         /* Retrieves CAN information and data, storing them in RxHeader and RxData respectively */
         HAL_FDCAN_GetRxMessage(hFDCAN, FDCAN_RX_FIFO0, &RxHeader, RxData);
 
         /* USER CODE BEGIN: CAN MSG */
 
         /* USER CODE END: CAN MSG */
 
         /* Calls the data handling function */
         CAN_Stream_ReceiveCallback(&RxHeader, RxData);
 
         /* Re-enables notification for new incoming messages */
         if (HAL_FDCAN_ActivateNotification(hFDCAN,
                 FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
             /* If it fails, calls the error handler */
             Error_Handler();
         }
     }
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 /*---- RECEIVED MESSAGE HANDLING -----------------------------------------------------------------------------*/ 
/**
 * @brief  Function for handling received messages
 * @param  hRxFDCAN: Handler with received frame information
 * @param  Buffer: Buffer with message data and information
 * @retval Function execution status
 */

  FDCAN_StatusTypedef CAN_Stream_ReceiveCallback(FDCAN_RxHeaderTypeDef *hRxFDCAN,
         uint8_t *Buffer) {
     /* If the identifier isn't part of the used IDs, the function aborts */
     if (hRxFDCAN->Identifier > CAN_IDS_NUMBER)
         /* If it fails, returns error */
         return FDCAN_ERROR;
 
     /* Variable for storing data size */
     uint8_t SIZE_DATA = hRxFDCAN->DataLength >> 16U;
 
     for (int i = SIZE_DATA; i < 8; i++)
         CAN_stream.Data_buf[hRxFDCAN->Identifier][i] = 0;
 
     if (SIZE_DATA <= INT_SIZE)
         SIZE_DATA = INT_SIZE;
 
     /* Switch case for proper data storage in memory */
     switch (SIZE_DATA) {
     case 0:
         /* Handling of positive int values */
         CAN_Storage_INT(hRxFDCAN->Identifier, Buffer);
         break;
     case INT_SIZE:
         /* Handling of positive int values */
         CAN_Storage_INT(hRxFDCAN->Identifier, Buffer);
         break;
     case FLOAT_SIZE:
         /* Handling of float values */
         CAN_Storage_FLOAT(hRxFDCAN->Identifier, Buffer);
         break;
     case DOUBLE_SIZE:
         /* Handling of double values */
         CAN_Storage_DOUBLE(hRxFDCAN->Identifier, Buffer);
         break;
     default:
         break;
     }
 
     for (int i = 0; i < 8; i++)
         Buffer[i] = 0;
 
     /* If everything works, returns OK */
     return FDCAN_OK;
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
/*---- CAN CONFIGURATION PARAMETERS --------------------------------------------------------------*/ 
/**
 * @brief  Configures the CAN, overwriting .IOC settings
 * @param  ***NONE***
 * @retval Function execution status
 */

  FDCAN_StatusTypedef CAN_Configure_Init() {
     /* Configures FDCAN parameters - REFER TO REPORT */
     hFDCAN->Instance = FDCAN2;
     hFDCAN->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
     hFDCAN->Init.Mode = FDCAN_MODO;
     hFDCAN->Init.AutoRetransmission = DISABLE;
     hFDCAN->Init.TransmitPause = DISABLE;
     hFDCAN->Init.ProtocolException = DISABLE;
     hFDCAN->Init.NominalPrescaler = 2;
     hFDCAN->Init.NominalSyncJumpWidth = 1;
     hFDCAN->Init.NominalTimeSeg1 = 2;
     hFDCAN->Init.NominalTimeSeg2 = 1;
     hFDCAN->Init.DataPrescaler = 1;
     hFDCAN->Init.DataSyncJumpWidth = 3;
     hFDCAN->Init.DataTimeSeg1 = 4;
     hFDCAN->Init.DataTimeSeg2 = 3;
     hFDCAN->Init.StdFiltersNbr = 0;
     hFDCAN->Init.ExtFiltersNbr = 0;
     hFDCAN->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
 
     /* These extra parameters aren't compatible with STM32G0, hence the "#ifdef" */
 #ifdef CHIP_STM32H7
     hFDCAN->Init.MessageRAMOffset = 0;
     hFDCAN->Init.RxFifo0ElmtsNbr = 1;
     hFDCAN->Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
     hFDCAN->Init.RxFifo1ElmtsNbr = 0;
     hFDCAN->Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
     hFDCAN->Init.RxBuffersNbr = 0;
     hFDCAN->Init.RxBufferSize = FDCAN_DATA_BYTES_8;
     hFDCAN->Init.TxEventsNbr = 0;
     hFDCAN->Init.TxBuffersNbr = 0;
     hFDCAN->Init.TxFifoQueueElmtsNbr = 32;
     hFDCAN->Init.TxElmtSize = FDCAN_DATA_BYTES_8;
 #endif
 
     /* Initializes CAN with defined parameters */
     if (HAL_FDCAN_Init(hFDCAN) != HAL_OK)
         /* If it fails, returns error */
         return FDCAN_ERROR;
     else
         /* If everything works, returns OK */
         return FDCAN_OK;
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
/*---- STORAGE BUFFERS CLEANUP --------------------------------------------------------------*/ 
/**
 * @brief  Initializes CAN message storage buffers
 * @param  ***NONE***
 * @retval ***NONE***
 */

  void CAN_Clean_Buffers(void) {
     /* Zeroes each position in the data array - Redundancy */
     for (uint16_t i = 0; i < CAN_IDS_NUMBER; i++) {
        /* Clears any memory garbage */
         for (uint8_t j = 0; j < 8; j++)
             CAN_stream.Data_buf[i][j] = 0U;
        /* Ensures no unallocated memory values can be retrieved */
//		CAN_stream->Type_buf[i] = FDCAN_FREE;
     }
 }
 
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

/*---- FDCAN COMMUNICATION INITIALIZATION --------------------------------------------------------------*/
/**
 * @brief  Initializes FDCAN communication
 * @param  ***NONE***
 * @retval Function execution status
 */

 FDCAN_StatusTypedef initialize_general_CAN(FDCAN_HandleTypeDef *can_ref) {
 
     initialize_CAN(hFDCAN, CAN_general_receive_callback,
             CAN_general_error_callback, &TxHeader);
 
     /* Calls the data storage array cleanup function */
     CAN_Clean_Buffers();
 
     /* Calls the CAN parameter configuration function */
     if (CAN_Configure_Init() != FDCAN_OK)
         /* If it fails, returns error */
         return FDCAN_ERROR;
 
     /* Starts CAN communication */
     if (HAL_FDCAN_Start(hFDCAN) != HAL_OK)
         /* If it fails, returns error */
         return FDCAN_ERROR;
 
     /* Enables notification for incoming messages */
     if (HAL_FDCAN_ActivateNotification(hFDCAN, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0)
             != HAL_OK)
         /* If it fails, returns error */
         return FDCAN_ERROR;
 
     /* Configures message sending parameters */
     TxHeader.IdType = FDCAN_STANDARD_ID; // IDENTIFIER TYPE - STANDARD OR EXTENDED
     TxHeader.TxFrameType = FDCAN_DATA_FRAME; // FRAME TYPE - DATA OR REMOTE
     TxHeader.DataLength = FDCAN_DLC_BYTES_8; // DATA SIZE - 0 TO 64 WORDS - 16 BIT SHIFT <<
     TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE; // ERROR INDICATOR - ACTIVE OR PASSIVE
     TxHeader.BitRateSwitch = FDCAN_BRS_OFF;	// INTERRUPT BIT - ON OR OFF
     TxHeader.FDFormat = FDCAN_FD_CAN;			// CAN TYPE - NORMAL OR FDCAN
     TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS; // SEND EVENT STORAGE - ON OR OFF
     TxHeader.MessageMarker = 0;				// MESSAGE MASK - 0 TO 0xFF
 
     /* If everything works, returns OK */
     return FDCAN_OK;
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
/*---- POSITIVE INT VALUE STORAGE --------------------------------------------------------------*/ 
/**
 * @brief  Function for storing positive int values, including type and size information
 * @param  Identifier: Message identifier
 * @param  Size: Required storage space
 * @param  Buffer: Pointer to buffer containing data and storage information
 * @retval ***NONE***
 */


  void CAN_Storage_INT(uint16_t Identifier, uint8_t *Buffer) {
     /* Stores variable type in CAN buffer */
     CAN_stream.Type_buf[Identifier] = FDCAN_INT;
     /* Stores value in allocated memory, byte by byte */
     for (int i = 0; i < INT_SIZE; i++)
         CAN_stream.Data_buf[Identifier][i] = Buffer[i];
 
     /* Stores value in allocated memory, byte by byte */
     for (int i = INT_SIZE; i < 8; i++)
         CAN_stream.Data_buf[Identifier][i] = 0;
 }
 
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

/*---- NEGATIVE INT VALUE STORAGE --------------------------------------------------------------*/
/**
 * @brief  Function for storing negative int values, including type and size information
 * @param  Identifier: Message identifier
 * @param  Size: Required storage space
 * @param  Buffer: Pointer to buffer containing data and storage information
 * @retval ***NONE***
 */


 void CAN_Storage_NEGATIVE(uint16_t Identifier, uint8_t *Buffer) {
 
 }
 
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

/*---- FLOAT VALUE STORAGE --------------------------------------------------------------*/

/**
 * @brief  Function for storing float values, including type and size information
 * @param  Identifier: Message identifier
 * @param  Size: Required storage space
 * @param  Buffer: Pointer to buffer containing data and storage information
 * @retval ***NONE***
 */

 void CAN_Storage_FLOAT(uint16_t Identifier, uint8_t *Buffer) {
     /* Stores variable type in CAN buffer */
     CAN_stream.Type_buf[Identifier] = FDCAN_FLOAT;
     /* Stores value in allocated memory, byte by byte */
     for (int i = 0; i < FLOAT_SIZE; i++)
         CAN_stream.Data_buf[Identifier][i] = Buffer[i];
 
     /* Stores value in allocated memory, byte by byte */
     for (int i = FLOAT_SIZE; i < 8; i++)
         CAN_stream.Data_buf[Identifier][i] = 0;
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
/*---- DOUBLE VALUE STORAGE --------------------------------------------------------------*/ 
/**
 * @brief  Function for storing double values, including type and size information
 * @param  Identifier: Message identifier
 * @param  Size: Required storage space
 * @param  Buffer: Pointer to buffer containing data and storage information
 * @retval ***NONE***
 */

 void CAN_Storage_DOUBLE(uint16_t Identifier, uint8_t *Buffer) {
     /* Stores variable type in CAN buffer */
     CAN_stream.Type_buf[Identifier] = FDCAN_DOUBLE;
     /* Stores value in allocated memory, byte by byte */
     for (int i = 0; i < DOUBLE_SIZE; i++)
         CAN_stream.Data_buf[Identifier][i] = Buffer[i];
 }
 
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

/*---- ACCESS TO STORED INT VALUES --------------------------------------------------------------*/
/**
 * @brief  Function to access int values stored in memory
 * @param  Identifier: Message identifier
 * @retval Stored integer value, returns "FDCAN_ERROR" if value isn't an int
 */

 int32_t CAN_Get_value(uint16_t Identifier) {
     if (CAN_stream.Type_buf[Identifier] != FDCAN_INT)
         /* If it fails, returns error */
         return FDCAN_ERROR;
 
     /* Variable to store value from memory */
     int32_t VALUE = 0;
 
     memcpy(&VALUE, CAN_stream.Data_buf[Identifier], 4);
 
     /* Returns stored value with correct sign */
     return VALUE;
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
/*---- ACCESS TO STORED FLOAT VALUES --------------------------------------------------------------*/
/**
 * @brief  Function to access float values stored in memory
 * @param  Identifier: Message identifier
 * @retval Stored float value, returns "FDCAN_ERROR" if value isn't a float
 */

 float CAN_Get_value_FLOAT(uint16_t Identifier) {
     if (CAN_stream.Type_buf[Identifier] != FDCAN_FLOAT)
         /* Caso de errado, retorna erro */
         return FDCAN_ERROR;
 
     float VALUE = 0;
 
     /*Recupera os dados armazenados na memória*/
     memcpy(&VALUE, CAN_stream.Data_buf[Identifier], 4);
 
     return VALUE;
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
/*---- ACCESS TO STORED DOUBLE VALUES --------------------------------------------------------------*/
/**
 * @brief  Function to access double values stored in memory
 * @param  Identifier: Message identifier
 * @retval Stored double value, returns "FDCAN_ERROR" if value isn't a double
 */
 
 double CAN_Get_value_DOUBLE(uint16_t Identifier) {
     if (CAN_stream.Type_buf[Identifier] != FDCAN_DOUBLE)
         /* If it fails, returns error */
         return FDCAN_ERROR;
 
     double VALUE = 0;
 
     /* Retrieves data stored in memory */
     memcpy(&VALUE, CAN_stream.Data_buf[Identifier], 8);
 
     return VALUE;
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
/*---- ADDING MESSAGES TO SEND BUFFER --------------------------------------------------------------*/
/**
 * @brief  Function to add a message to the send buffer
 * @param  Identifier: Message identifier
 * @param  Buffer: Data buffer to send
 * @param  Size: Number of bytes to send
 * @retval Function execution status
 */

  FDCAN_StatusTypedef CAN_TxData(uint16_t Identifier, uint64_t Buffer,
         uint8_t Size) {
     uint64_t TIME = HAL_GetTick();
     uint64_t *pBuffer = &Buffer;
     uint32_t SIZE_DATA = Size << 16U;
 
 //	if (Size == INT_SIZE) {
 //		for (int i = 0; i < 4; i++)
 //			if (Buffer >> 8 * i == 0) {
 //				SIZE_DATA = i << 16U;
 //				break;
 //			}
 //	}
 
     /* Stores message identifier in information struct (TxHeader) */
     TxHeader.Identifier = Identifier;
 
     TxHeader.DataLength = SIZE_DATA;
 
     while (HAL_FDCAN_GetTxFifoFreeLevel(hFDCAN) == 0)
         if (HAL_GetTick() - TIME > TIME_TO_BREAK)
             return FDCAN_TIMEOUT;
 
     /* Envia os dados recebidos na chamada (data) pela CAN, de acordo com as informações de TxHeader */
     if (HAL_FDCAN_AddMessageToTxFifoQ(hFDCAN, &TxHeader, (uint8_t*) pBuffer)
             != HAL_OK)
         /* If it fails, returns error */
         return FDCAN_ERROR;
 
     /* If everything works, returns OK */
     return FDCAN_OK;
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
/*---- INT PROCESSING AND SENDING --------------------------------------------------------------*/ 
/**
 * @brief  Function for processing and sending ints values via bus
 * @param  Identifier: Message identifier
 * @param  Value: Int value to send via bus
 * @retval Function execution status
 */

  FDCAN_StatusTypedef CAN_Send(uint16_t Identifier, int32_t Value) {
     uint32_t Value_Buf = 0;
 
     memcpy(&Value_Buf, &Value, 4);
 
     if (CAN_TxData(Identifier, Value_Buf, INT_SIZE) != FDCAN_OK)
          /* If it fails, returns error */
         return FDCAN_ERROR;
 
     /* If everything works, returns OK */
     return FDCAN_OK;
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
/*------------------ FLOAT PROCESSING AND SENDING ------------------------------------------*/

/**
 * @brief  Function for processing and sending integer values via bus
 * @param  Identifier: Message identifier
 * @param  Value: Float value to send via bus
 * @retval Function execution status
 */

  FDCAN_StatusTypedef CAN_Send_Float(uint16_t Identifier, float Value) {
 
     uint32_t Value_Buf = 0;
 
     memcpy(&Value_Buf, &Value, 4);
 
     if (CAN_TxData(Identifier, Value_Buf, FLOAT_SIZE) != FDCAN_OK)
         /* If it fails, returns error */
         return FDCAN_ERROR;
 
      /* If everything works, returns OK */
     return FDCAN_OK;
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
/*------------------ DOUBLE PROCESSING AND SENDING ----------------------
 
/**
 * @brief  Function for processing and sending integer values via bus
 * @param  Identifier: Message identifier
 * @param  Value: Double value to send via bus
 * @retval Function execution status
*/

 FDCAN_StatusTypedef CAN_Send_Double(uint16_t Identifier, double Value) {
     uint64_t Value_Buf = 0;
 
     memcpy(&Value_Buf, &Value, 8);
 
     if (CAN_TxData(Identifier, Value_Buf, DOUBLE_SIZE) != FDCAN_OK)
         /* If it fails, returns error */
         return FDCAN_ERROR;
 
     /* If everything works, returns OK */
     return FDCAN_OK;
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 // Callback for BUSOFF error in general can.
 static void CAN_general_error_callback(FDCAN_HandleTypeDef* hfdcan,
                                        uint32_t ErrorStatusITs) {
     if (ErrorStatusITs | FDCAN_IT_BUS_OFF) {
         // Sets the flag for datalogging
         osEventFlagsSet(e_ECU_control_flagsHandle, GENERAL_BUS_OFF_ERROR_FLAG);
         // Clear INIT bit, so can communication is reestablished
         CLEAR_BIT(hfdcan->Instance->CCCR, FDCAN_CCCR_INIT);
     }
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /* USER CODE END PF */ 