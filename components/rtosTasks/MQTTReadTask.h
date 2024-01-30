#ifndef MAIN_RTOSTASKS_MQTTREADTASK_H_
#define MAIN_RTOSTASKS_MQTTREADTASK_H_


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern TaskHandle_t sensorMQTTTaskHandle;

/*
 *  method for creating task that responds to MQTT request
 */
int CreateMQTTReadTask(void);


#endif /* MAIN_RTOSTASKS_MQTTREADTASK_H_ */
