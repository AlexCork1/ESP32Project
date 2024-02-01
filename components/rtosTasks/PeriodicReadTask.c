#include <MQTT_GenerateMessage.h>
#include <TriggerType.h>
#include "PeriodicReadTask.h"
#include "CommonVariablesAllTasks.h"
#include "SensorData.h"
#include "SensorTempHum.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

TaskHandle_t sensorPeriodicTaskHandle;
const uint32_t PERIODIC_TASK_FREQUENCY_MS = 5000;

/*
 * Task used for 5s periodically reds from sensor.
 * First is reads data from sensor and converts it to MQTT_Message
 * Sends the data to publish task over queu
 */
void Task_PeriodicRead(void *params)
{
	TickType_t xLastWakeTime;
	const uint32_t freq = PERIODIC_TASK_FREQUENCY_MS / portTICK_PERIOD_MS;
	const uint32_t small_delay_ms = 20 / portTICK_PERIOD_MS;

	SensorData data;
	MQTT_MESSAGE message;

	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		//get latest data from sensor
		Sensor_GetData(&data);
		ESP_LOGI("Task_PeriodicRead", "Periodic task : message send to queue");
		//convert it to json
		GeneratePublishMessage(&message,PERIODIC_TIMER, &data);

		//if message was send to queue correctly, we put task to sleep. Otherwise we try again
		if(xQueueSend(jsonQueue, &message, 0) == pdTRUE)
			vTaskDelayUntil(&xLastWakeTime, freq);

		//small delay to allow other tasks to execute
		vTaskDelay(small_delay_ms);
	}
}

/*
 *  method for creating task that executed every 5 seconds
 *  Out:
 *  0 - everything is ok
 *  1 - task creation has failed
 */
int CreatePeriodicTask(void)
{
	int response;
	response = xTaskCreatePinnedToCore(
			Task_PeriodicRead, //task function
			"Sensor_Periodic", //task name
			4096, //stack size
			NULL,//parameters
			3,	//priority (higher is more important)
			&sensorPeriodicTaskHandle, //task handle
			1); //task core

	if (response != pdPASS)
	{
		ESP_LOGE("InitFreeRTOSStructs", "Error : Task_PeriodicRead");
		return -1;
	}
	return 0;
}
