#ifndef MAIN_RTOSTASKS_READSENSORTASK_H_
#define MAIN_RTOSTASKS_READSENSORTASK_H_


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern TaskHandle_t sensorUpdateTaskHandle;

/*
 *  method for creating task that actuall read from sensor
 */
int CreateReadSensorTask(void);



#endif /* MAIN_RTOSTASKS_READSENSORTASK_H_ */
