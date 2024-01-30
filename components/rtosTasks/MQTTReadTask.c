#include "TriggerType.h"
#include "MQTTReadTask.h"
#include "CommonVariablesAllTasks.h"
#include "SensorData.h"
#include "SensorTempHum.h"
#include "MQTT_GenerateMessage.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

TaskHandle_t sensorMQTTTaskHandle;

/*
 * Task used for handling MQTT request. Semaphore is used for synchronization between task and MQTT handler
 * First is reads data from sensor and converts it to MQTT_Message
 * Sends the data to publish task over queu
 */
void Task_MQTTRead_Command(void *params)
{
	const uint32_t wait_for_sem_ms = 10 / portTICK_PERIOD_MS;
	const uint32_t frequency_timer_ms = 50 / portTICK_PERIOD_MS;

	SensorData data;
	MQTT_MESSAGE message;

	uint8_t newMessageArrived = 0;

	while(1)
	{
		// Wait for the semaphore signal indicating a message was received
        if (xSemaphoreTake(message_received_sem, wait_for_sem_ms) == pdTRUE)
        {
        	//get data from sensor
    		Sensor_GetData(&data);
    		//generate json format message
    		GeneratePublishMessage(&message,CLOUD_REQUEST, &data);
    		newMessageArrived = 1;
    		ESP_LOGI("Task_MQTTRead_Command", "Message received from MQTT");
        }

        if (newMessageArrived == 1)
        {
    		//try to send message over queue to publish task
    		if(xQueueSend(jsonQueue, &message, 0) == pdTRUE)
    			newMessageArrived = 0;
        }
		vTaskDelay(frequency_timer_ms);
	}
}

/*
 *  method for creating task that responds to MQTT request
 *  Out:
 *  0 - everything is ok
 *  1 - task creation has failed
 */
int CreateMQTTReadTask(void)
{
	int response;
	response = xTaskCreatePinnedToCore(
				Task_MQTTRead_Command, //task function
				"Sensor_MQTT read", //task name
				2048, //stack size
				NULL,//parameters
				3,	//priority (higher is more important)
				&sensorMQTTTaskHandle, //task handle
				1); //task core

	if (response != pdPASS)
	{
		ESP_LOGE("InitFreeRTOSStructs", "Error : Task_MQTTRead_Command");
		return -1; //should we handle it any other way?
	}
	return 0;
}
