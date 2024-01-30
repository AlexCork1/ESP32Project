/*
 * Declaring extern variables used by all tasks
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

//used for signaling between MQTT handle request and Task_MQTTRead_Command
extern SemaphoreHandle_t message_received_sem;

//globally defined MQTT broker used by Task_MQTT_Publish
extern esp_mqtt_client_handle_t clientMQTTBroker;

//queue in which all tasks send data and is then read by Task_MQTT_Publish
extern QueueHandle_t jsonQueue;



#endif /* MAIN_RTOSTASKS_COMMONVARIABLESALLTASKS_H_ */
