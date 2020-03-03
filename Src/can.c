/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "ecu.h"
#include "eeprom.h"
#include "gpio.h"

/* USER CODE BEGIN 0 */

uint8_t* idRx;
uint8_t* vetRx;
uint8_t vetTx[__DLC__];
uint8_t retry_receive=0, erro_rec =0, erro_trans =0 ;

uint8_t can_flag_er=0;

uint32_t id_temp=0;
uint8_t vet_temp[8];
uint16_t iter = 0, temp_can = -1;


/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 90; //36 -> 125k, 90 -> 50k, 225 ->20k
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SJW = CAN_SJW_1TQ;
  hcan.Init.BS1 = CAN_BS1_6TQ;
  hcan.Init.BS2 = CAN_BS2_1TQ;
  hcan.Init.TTCM = DISABLE;
  hcan.Init.ABOM = ENABLE;
  hcan.Init.AWUM = DISABLE;
  hcan.Init.NART = ENABLE;
  hcan.Init.RFLM = DISABLE;
  hcan.Init.TXFP = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    /**CAN GPIO Configuration    
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_CAN1_2();

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN GPIO Configuration    
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_HP_CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/*##-2- Configure the CAN Filter ###########################################*/
void CAN_ConfigFilter(void){
	CAN_FilterConfTypeDef  sFilterConfig;
	sFilterConfig.FilterNumber = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;       //FILTRO CONFIGURADO PARA NAO DAR ERRO
	sFilterConfig.FilterMaskIdHigh = 0x0000;  //MAS N�O ESTA SENDO USADO
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = 0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.BankNumber = 14;

	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)  // RETORNA O STATUS DA FUN��O
	{
	  /* Filter configuration Error */
	  _Error_Handler(__FILE__, __LINE__);
	}

}


void CAN_ConfigFrames(void){
	static CanTxMsgTypeDef        TxMessage; // Struct de defini��o da estrutura da mensagem CAN Tx
	static CanRxMsgTypeDef        RxMessage;

	Init_RxMes(&RxMessage);

	hcan.pTxMsg = &TxMessage;
	hcan.pRxMsg = &RxMessage;

	/*##-3- Configure Transmission process #####################################*/
	// CONFIGURA A STRUCT TxMessage
	hcan.pTxMsg->StdId = 0x300; //Specifies the standard identifier
	hcan.pTxMsg->ExtId = 0x01; //Specifies the extended identifier.
	hcan.pTxMsg->RTR = CAN_RTR_DATA; //FRAME DE DADO|FRAME REMOTA / Specifies the type of frame for the message that will be transmitted.
	hcan.pTxMsg->IDE = CAN_ID_STD;//STANDARD ID 11b|EXTENDED ID 29b /Specifies the type of identifier for the message that will be transmitted.
	hcan.pTxMsg->DLC = 8; //Specifies the length of the frame that will be transmitted.
}

/*##-2- Start the Reception process and enable reception interrupt #########*/
void CAN_Receive_Init(){

  if(HAL_CAN_Receive_IT(&hcan, CAN_FIFO0) != HAL_OK)
  	  Error_Handler();
//  if(HAL_CAN_Receive_IT(&hcan, CAN_FIFO1) != HAL_OK)
//  	  Error_Handler();
}

void CAN_Receive_Handler(){
	//loopback
	//CAN_Transmit(hcan.pRxMsg->Data,hcan.pRxMsg->StdId);
  if(HAL_CAN_Receive_IT(&hcan, CAN_FIFO0) != HAL_OK)
	  Error_Handler();
//  if(HAL_CAN_Receive_IT(&hcan, CAN_FIFO1) != HAL_OK)
//  	  Error_Handler();
}


void CAN_Transmit(uint8_t *vet, uint32_t id){
	/* Set the data to be transmitted, from array *vet */
	hcan.pTxMsg->StdId = id; //Specifies the standard identifier
	for(uint8_t i=0; i < hcan.pTxMsg->DLC; i++)
		hcan.pTxMsg->Data[i] = vet[i];
	/*##-3- Start the Transmission process ###############################*/
	HAL_StatusTypeDef trans_status = HAL_CAN_Transmit_IT(&hcan);
	//Error handler
	if (trans_status != HAL_OK)
	{
		//HAL_CAN_ErrorCallback(&hcan);
		can_flag_er=1;
//		seta_leds(0);
		erro_trans++;
	}
	else if(can_flag_er ==1)
	{
		can_flag_er = 0;
//		seta_leds(led_conf);
	}

}

/**
  * @brief  Initializes a Rx Message.
  * @param  CanRxMsgTypeDef *RxMessage
  * @retval None
  */
void Init_RxMes(CanRxMsgTypeDef *RxMessage)
{
  uint8_t i = 0;

  RxMessage->StdId = 0x321;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (i = 0;i < 8;i++)
  {
    RxMessage->Data[i] = 0x00;
  }
}

void tx_temporizado(){



	if(iter>temp_can){
		iter = 0;
		vet_temp[0] = (hcan.Instance->ESR) & 0b111;
		vet_temp[1] = (hcan.Instance->ESR>>4) & 0b111;
		vet_temp[2] = ((hcan.Instance->ESR)>>16) & 0xff;
		vet_temp[3] = ((hcan.Instance->ESR)>>24) & 0xff;
		CAN_Transmit(vet_temp, id_temp);
	}
	iter++;

}


// when # of CAN frames exceeds FIFO capacity reception stops once error-flag is set
// this clears error-flag and enables CAN Rx interrupt again
// called by CAN_IRQHandler at the end of execution
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
	Error_Diagnostics_Write(hcan);

	  // For CAN Rx frames received in FIFO number 0.
		__HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV0);
	  HAL_CAN_Receive_IT(hcan, CAN_FIFO0);
	  __HAL_CAN_FIFO_RELEASE(hcan, CAN_FIFO0);



	  // For CAN Rx frames received in FIFO number 1.
//	  __HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOV1);
//	  __HAL_CAN_Receive_IT(hcan, CAN_FIFO1);
	  __HAL_CAN_RESET_HANDLE_STATE(hcan);
	__HAL_CAN_ENABLE_IT(hcan, CAN_IT_EWG |
	                               CAN_IT_EPV |
	                               CAN_IT_BOF |
	                               CAN_IT_LEC |
	                               CAN_IT_ERR |
	                               CAN_IT_FMP0|
	                               CAN_IT_FOV0|
	                               CAN_IT_FMP1|
	                               CAN_IT_FOV1|
	                               CAN_IT_TME  );
	__HAL_UNLOCK(hcan);

	//MX_CAN_Init();
	//CAN_ConfigFilter();
	//CAN_ConfigFrames();
	//CAN_Receive_Init();



}

void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef * hcan){}




/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
