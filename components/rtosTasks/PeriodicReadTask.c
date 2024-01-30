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

void Task_PeriodicRead(void *params)
{
	TickType_t xLastWakeTime;
	const uint32_t freq = PERIODIC_TASK_FREQUENCY_MS / portTICK_PERIOD_MS;

	SensorData data;
	MQTT_MESSAGE message;

	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		Sensor_GetData(&data);
		ESP_LOGI("Task_PeriodicRead", "Periodic task : message send to queue");
		GeneratePublishMessage(&message,PERIODIC_TIMER, &data);

		//if message was send to queue correctly, we put task to sleep. Otherwise we try again
		if(xQueueSend(jsonQueue, &message, 0) == pdTRUE)
			vTaskDelayUntil(&xLastWakeTime, freq);
	}
}

int CreatePeriodicTask(void)
{
	int response;
	response = xTaskCreatePinnedToCore(
			Task_PeriodicRead, //task function
			"Sensor_Periodic", //task name
			2048, //stack size
			NULL,//parameters
			3,	//priority (higher is more important)
			&sensorPeriodicTaskHandle, //task handle
			1); //task core

	if (response != pdPASS){
		//TODO somethins is wrong - we should tell someone something
		ESP_LOGE("InitFreeRTOSStructs", "Error : Task_PeriodicRead");
		while(1);//quick fix
	}
	return 0;
}
