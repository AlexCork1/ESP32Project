#include "ReadSensorTask.h"
#include "CommonVariablesAllTasks.h"
#include "SensorTempHum.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

TaskHandle_t sensorUpdateTaskHandle;
#define READ_TEMPERATURE_SENSOR_FREQUENCY_MS 5000

/*
 * Task that initializes and does actual reading from sensor
 * This task (could/can) be joined with Task_PeriodicRead to make application smaller
 */

void Task_GetTemperatureAndHumidity(void* params)
{
	TickType_t xLastWakeTime;
	const uint32_t freq = READ_TEMPERATURE_SENSOR_FREQUENCY_MS / portTICK_PERIOD_MS;

	Sensor_Init(5);

	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		ESP_LOGI("Task_GetTemperatureAndHumidity", "Task_GetTemperatureAndHumidity\n");
		//get latest data from sensor
		Sensor_UpdateData();
		//wait for tmie interval
        vTaskDelayUntil(&xLastWakeTime, freq);
	}
}

/*
 *  method for creating periodic read task
 *  Out:
 *  0 - everything is ok
 *  1 - task creation has failed
 */
int CreateReadSensorTask(void){
	BaseType_t response;
	response = xTaskCreatePinnedToCore(
			Task_GetTemperatureAndHumidity, //task function
			"SensorUpdateData", //task name
			4096, //stack size
			NULL,//parameters
			5,	//priority (higher is more important)
			&sensorUpdateTaskHandle, //task handle
			1); //task core

	if (response != pdPASS)
	{
		ESP_LOGE("InitFreeRTOSStructs", "Error : Task_GetTemperatureAndHumidity");
		return -1;
	}

	return 0;
}
