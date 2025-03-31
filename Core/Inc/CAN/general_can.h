/*
 *      Communication via CAN - general_can.h
 *
 *      Created on: JUN 13, 2023
 *      Author: Gabriel Luiz
 *      Contact: (31) 97136-4334 || gabrielluiz.eletro@gmail.com
 */

#ifndef INC_GENERAL_CAN_H_
#define INC_GENERAL_CAN_H_

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
 
 /* USADO PARA REGISTRAR UM BUFFER DA CAN */
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
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- CALLBACK DE RECEBIMENTO DA CAN --------------------------------------------------------------------------------*/
 
 /**
  * @brief  Função chamada quando detectado uma mensagem no barramento da CAN
  * @param  hfdcan: Handle da CAN || normalmente "hfdcan1"
  * @param  RxFifo0ITs: FIFO em que foi detectado a mensagem
  * @retval ***NONE***
  */
 //__weak void CAN_general_receive_callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- TRATAMENTO DE MENSAGENS RECEBIDAS -----------------------------------------------------------------------------*/
 
 /**
  * @brief  Função de tratamento das mensagens recebidas
  * @param  hRxFDCAN: Handler com as innformações do flame recebido
  * @param  Buffer: Buffer com os dados e informações da mensagem
  * @retval Status de execução da função
  */
 FDCAN_StatusTypedef CAN_Stream_ReceiveCallback(FDCAN_RxHeaderTypeDef *hRxFDCAN, uint8_t *Buffer);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- PARAMETROS DE CONFIGURAÇÃO DA CAN --------------------------------------------------------------*/
 
 /**
  * @brief  Configura a CAN, overwrite das configurações do .IOC
  * @param  ***NONE***
  * @retval Status de execução da função
  */
 FDCAN_StatusTypedef CAN_Configure_Init(void);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- LIMPEZA DOS BUFFERS DE ARMAZENAMENTO --------------------------------------------------------------*/
 
 /**
  * @brief  Inicialização dos buffers de armazenamento das mensagens da CAN
  * @param  ***NONE***
  * @retval ***NONE***
  */
 void CAN_Clean_Buffers(void);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- INÍCIO DA COMUNICAÇÃO VIA FDCAN --------------------------------------------------------------*/
 
 /**
  * @brief  Inicialização da comunicação via FDCAN
  * @param  ***NONE***
  * @retval Status de execução da função
  */
 FDCAN_StatusTypedef initialize_general_CAN(FDCAN_HandleTypeDef* can_ref);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- ARMAZENAMENTO DE VALORES INTEIROS --------------------------------------------------------------*/
 
 /**
  * @brief  Função para armazenamento de valores inteiros, assim como informação de tipo
  * @param  Identifier: Identificador da mensagem
  * @param  Buffer: Ponteiro para os buffer que contém os dados e as informações para seu armazenamento
  * @retval ***NONE***
  */
 void CAN_Storage_INT(uint16_t Identifier, uint8_t *Buffer);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- ARMAZENAMENTO DE VALORES FLOATS --------------------------------------------------------------*/
 
 /**
  * @brief  Função para armazenamento de valores floats, assim como informação de tipo
  * @param  Identifier: Identificador da mensagem
  * @param  Buffer: Ponteiro para os buffer que contém os dados e as informações para seu armazenamento
  * @retval ***NONE***
  */
 void CAN_Storage_FLOAT(uint16_t Identifier, uint8_t *Buffer);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- ARMAZENAMENTO DE VALORES DOUBLES --------------------------------------------------------------*/
 
 /**
  * @brief  Função para armazenamento de valores doubles, assim como informação de tipo
  * @param  Identifier: Identificador da mensagem
  * @param  Buffer: Ponteiro para os buffer que contém os dados e as informações para seu armazenamento
  * @retval ***NONE***
  */
 void CAN_Storage_DOUBLE(uint16_t Identifier, uint8_t *Buffer);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- ACESSO AOS VALORES INTEIROS ARMAZENADOS --------------------------------------------------------------*/
 
 /**
  * @brief  Função para acesso aos valores inteiros armazenados na memória
  * @param  Identifier: Identificador da mensagem
  * @retval Valor inteiro armazenado, caso o valor não seja um inteiro retorna "FDCAN_ERRO"
  */
 int32_t CAN_Get_value(uint16_t Identifier);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- ACESSO AOS VALORES FLOATS ARMAZENADOS --------------------------------------------------------------*/
 
 /**
  * @brief  Função para acesso aos valores floats armazenados na memória
  * @param  Identifier: Identificador da mensagem
  * @retval Valor float armazeenado, caso o valor não seja um float retorna "FDCAN_ERRO"
  */
 float CAN_Get_value_FLOAT(uint16_t Identifier);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- ACESSO AOS VALORES DOUBLES ARMAZENADOS --------------------------------------------------------------*/
 
 /**
  * @brief  Função para acesso aos valores doubles armazenados na memória
  * @param  Identifier: Identificador da mensagem
  * @retval Valor double armazenado, caso o valor não seja um double retorna "FDCAN_ERRO"
  */
 double CAN_Get_value_DOUBLE(uint16_t Identifier);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- INCLUSÃO DE MENSAGENS NO BUFFER DE ENVIO --------------------------------------------------------------*/
 
 /**
  * @brief  Função para adicionar uma mensagem no buffer de envio
  * @param  Identifier: Identificador da mensagemss
  * @param  Buffer: Buffer de dados para envio
  * @param  Size: Quantidade de byte para envio
  * @retval Status de execução da função
  */
 FDCAN_StatusTypedef CAN_TxData(uint16_t Identifier, uint64_t Buffer, uint8_t Size);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- TRATAMENTO E ENVIO DE INTEIROS --------------------------------------------------------------*/
 
 /**
  * @brief  Função para tratamento e envio de valores inteiros pelo barramento
  * @param  Identifier: Identificador da mensagem
  * @param  Value: Valor inteiro para envio pelo barrameto
  * @retval Status de execução da função
  */
 FDCAN_StatusTypedef CAN_Send(uint16_t Identifier, int32_t Value);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- TRATAMENTO E ENVIO DE FLOATS --------------------------------------------------------------*/
 
 /**
  * @brief  Função para tratamento e envio de valores inteiros pelo barramento
  * @param  Identifier: Identificador da mensagem
  * @param  Value: Valor float para envio pelo barrameto
  * @retval Status de execução da função
  */
 FDCAN_StatusTypedef CAN_Send_Float(uint16_t Identifier, float Data);
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /*---- TRATAMENTO E ENVIO DE DOUBLES --------------------------------------------------------------*/
 
 /**
  * @brief  Função para tratamento e envio de valores inteiros pelo barramento
  * @param  Identifier: Identificador da mensagem
  * @param  Value: Valor double para envio pelo barrameto
  * @retval Status de execução da função
  */
 FDCAN_StatusTypedef CAN_Send_Double(uint16_t Identifier, double Data);
 
 #endif /* CAN_LOG_H_ */ 