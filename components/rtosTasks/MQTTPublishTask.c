#include <MQTT_GenerateMessage.h>
#include "MQTTPublishTask.h"
#include "CommonVariablesAllTasks.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include <stdint.h>

TaskHandle_t mqttPublishTaskHandle;

/*
 * Task used for publishing data to MQTT broker. This happens only here and nowhere else
 * Content for publishing is send from tasks using queue
 */
void Task_MQTT_Publish(void* params)
{
	MQTT_MESSAGE message;
	const uint32_t time_to_wait_ms = 100 / portTICK_PERIOD_MS;
	while(1)
	{
		if(xQueueReceive(jsonQueue, &message, 0) == pdTRUE)
		{
			if (clientMQTTBroker == NULL) {
				ESP_LOGE("MQTT client broker", "MQTT client broker is NULL\n");
				return;
			}

			int ret = esp_mqtt_client_publish(clientMQTTBroker, CONFIG_MQTT_PUBLISH_TOPIC, message.text, 0, 0, 0); //len is calculated in function
			if(ret < 0)
				ESP_LOGE("MQTT client broker", "MQTT publish message was unsuccessful with error code: %d\n", ret);
			else
				ESP_LOGI("MQTT client broker", "MQTT publish message was successful. Message: %s\n", message.text);
		}
		vTaskDelay(time_to_wait_ms);
	}
}


/*
 *  method for creating publish task
 *  Out:
 *  0 - everything is ok
 *  1 - task creation has failed
 */
int CreateMQTTPublishTask(void)
{
	BaseType_t response;
	response = xTaskCreatePinnedToCore(
			Task_MQTT_Publish, //task function
			"MQTT_Publish", //task name
			2048, //stack size
			NULL,//parameters
			4,	//priority (higher is more important)
			&mqttPublishTaskHandle, //task handle
			1); //task core

	if (response != pdPASS){
		ESP_LOGE("InitFreeRTOSStructs", "Error : Task_MQTT_Publish");
		return -1; //should we handle it any other way?
	}

	return 0;
}
