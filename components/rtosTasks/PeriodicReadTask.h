#ifndef MAIN_RTOSTASKS_PERIODICREADTASK_H_
#define MAIN_RTOSTASKS_PERIODICREADTASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern TaskHandle_t sensorPeriodicTaskHandle;
int CreatePeriodicTask(void);


#endif /* MAIN_RTOSTASKS_PERIODICREADTASK_H_ */
