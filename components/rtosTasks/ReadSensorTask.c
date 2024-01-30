#include "ReadSensorTask.h"
#include "CommonVariablesAllTasks.h"
#include "SensorTempHum.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

TaskHandle_t sensorUpdateTaskHandle;
#define READ_TEMPERATURE_SENSOR_FREQUENCY_MS 5000

//this task can be joined with Task_PeriodicRead
void Task_GetTemperatureAndHumidity(void* params)
{
	TickType_t xLastWakeTime;
	const uint32_t freq = READ_TEMPERATURE_SENSOR_FREQUENCY_MS / portTICK_PERIOD_MS;

	Sensor_Init();

	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		ESP_LOGI("Task_GetTemperatureAndHumidity", "Task_GetTemperatureAndHumidity\n");
		Sensor_UpdateData();
        vTaskDelayUntil(&xLastWakeTime, freq);
	}
}

int CreateReadSensorTask(void){
	BaseType_t response;
	response = xTaskCreatePinnedToCore(
			Task_GetTemperatureAndHumidity, //task function
			"SensorUpdateData", //task name
			2048, //stack size
			NULL,//parameters
			5,	//priority (higher is more important)
			&sensorUpdateTaskHandle, //task handle
			1); //task core

	if (response != pdPASS){
		//TODO somethins is wrong - we should tell someone something
		ESP_LOGE("InitFreeRTOSStructs", "Error : Task_GetTemperatureAndHumidity");
		while(1);//quick fix
	}

	return 0;
}
