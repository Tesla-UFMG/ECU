/*
 * cmsis_extra.h
 *
 *  Created on: 10 de jul de 2021
 *      Author: renanmoreira
 */

#ifndef INC_CMSIS_EXTRA_CMSIS_EXTRA_H_
#define INC_CMSIS_EXTRA_CMSIS_EXTRA_H_

#include "cmsis_os2.h"

/// Put a Message into a Queue and overwrites if it's full
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \param[in]     msg_ptr       pointer to buffer with message to put into a queue.
/// \param[in]     msg_prio      message priority.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueuePutOverwrite (osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio);



#endif /* INC_CMSIS_EXTRA_CMSIS_EXTRA_H_ */
