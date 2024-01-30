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
        if (xSemaphoreTake(message_received_sem, wait_for_sem_ms) == pdTRUE) {
    		Sensor_GetData(&data);
    		GeneratePublishMessage(&message,CLOUD_REQUEST, &data);
    		newMessageArrived = 1;
    		ESP_LOGI("Task_MQTTRead_Command", "Message received from MQTT");
        }

        if (newMessageArrived == 1)
        {
    		//try to send message to queue
    		if(xQueueSend(jsonQueue, &message, 0) == pdTRUE)
    			newMessageArrived = 0;
        }
		vTaskDelay(frequency_timer_ms);
	}
}

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

	if (response != pdPASS){
		//TODO somethins is wrong - we should tell someone something
		ESP_LOGE("InitFreeRTOSStructs", "Error : Task_MQTTRead_Command");
		while(1);//quick fix
	}
	return 0;
}
