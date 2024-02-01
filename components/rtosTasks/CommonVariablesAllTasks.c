/*
 * Definition of all extern variables used by all tasks
 */
#include "CommonVariablesAllTasks.h"

SemaphoreHandle_t message_received_sem;
SemaphoreHandle_t message_ota_mqtt;

esp_mqtt_client_handle_t clientMQTTBroker;

QueueHandle_t jsonQueue;
