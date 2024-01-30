#ifndef MAIN_RTOSTASKS_MQTTPUBLISHTASK_H_
#define MAIN_RTOSTASKS_MQTTPUBLISHTASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern TaskHandle_t mqttPublishTaskHandle;

int CreateMQTTPublishTask(void);

#endif /* MAIN_RTOSTASKS_MQTTPUBLISHTASK_H_ */
