/*
 * ReadUserButton.h
 *
 *  Created on: 29 Jan 2024
 *      Author: Ales
 */

#ifndef MAIN_RTOSTASKS_READUSERBUTTONTASK_H_
#define MAIN_RTOSTASKS_READUSERBUTTONTASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern TaskHandle_t sensorButtonTaskHandle;

/*
 *  method for creating task that reads button and handles it's request
 */
int CreateReadUserButtonTask(void);



#endif /* MAIN_RTOSTASKS_READUSERBUTTONTASK_H_ */
