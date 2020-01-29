/*
 * flash.c
 *
 *  Created on: 27 de mar de 2018
 *      Author: User
 */

#include "flash.h"

#define EraseTimeout	            ((uint32_t)0x00000FFF)
#define ProgramTimeout          	((uint32_t)0x0000000F)

#define CR_PER_Reset   				((uint32_t)0x00001FFD)
#define CR_PG_Reset             	((uint32_t)0x00001FFE)



HAL_StatusTypeDef FLASH_ErasePage(uint32_t Page_Address)
{
  HAL_StatusTypeDef status = HAL_OK;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Page_Address));/**/
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);

  if(status == HAL_OK)
  {
    /* if the previous operation is completed, proceed to erase the page */
    FLASH->CR|= FLASH_CR_PER; //CR_PER_Set; /*!< 0x00000002 */
    FLASH->AR = Page_Address;
    FLASH->CR|= FLASH_CR_STRT; //CR_STRT_Set; /*!< 0x00000040 */

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);/**/
    if(status != HAL_TIMEOUT)/**/
    {
      /* if the erase operation is completed, disable the PER Bit */
      FLASH->CR &= CR_PER_Reset;
    }
  }
  /* Return the Erase Status */
  return status;
}



HAL_StatusTypeDef FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
  HAL_StatusTypeDef status = HAL_OK;/**/
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));/**/
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == HAL_OK)/**/
  {
    /* if the previous operation is completed, proceed to program the new data */
    FLASH->CR |= FLASH_CR_PG;/**/

    *(__IO uint16_t*)Address = Data;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != HAL_TIMEOUT)
    {
      /* if the program operation is completed, disable the PG Bit */
      FLASH->CR &= CR_PG_Reset;
    }
  }
  /* Return the Program Status */
  return status;
}
