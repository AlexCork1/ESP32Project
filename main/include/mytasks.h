/*
 * mytasks.h
 *
 *  Created on: 29 Jan 2024
 *      Author: Ales
 */

#ifndef MAIN_MYTASKS_H_
#define MAIN_MYTASKS_H_


#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

void InitFreeRTOSStructs(void);
extern SemaphoreHandle_t message_received_sem;


#endif /* MAIN_MYTASKS_H_ */
