#include "CommonVariablesAllTasks.h"

SemaphoreHandle_t message_received_sem;
esp_mqtt_client_handle_t clientMQTTBroker;
QueueHandle_t jsonQueue;
