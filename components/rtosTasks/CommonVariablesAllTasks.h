/*
 * AllTasks.h
 *
 *  Created on: 29 Jan 2024
 *      Author: Ales
 */

#ifndef MAIN_RTOSTASKS_COMMONVARIABLESALLTASKS_H_
#define MAIN_RTOSTASKS_COMMONVARIABLESALLTASKS_H_


#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/idf_additions.h"
#include "esp_log.h"

#include "mqtt_client.h"

extern SemaphoreHandle_t message_received_sem;
extern esp_mqtt_client_handle_t clientMQTTBroker;
extern QueueHandle_t jsonQueue;



#endif /* MAIN_RTOSTASKS_COMMONVARIABLESALLTASKS_H_ */
