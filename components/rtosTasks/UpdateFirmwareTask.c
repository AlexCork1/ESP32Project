#include "UpdateFirmwareTask.h"
#include "CommonVariablesAllTasks.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

TaskHandle_t firmwareUpdateTaskHandle;

/*
 * Task that will update firmware on request
 *
 */
void Task_MQTTRead_UpdateFirmware(void *params)
{
	//register to mqtt message
	while(1)
	{
		ESP_LOGI("Task_MQTTRead_UpdateFirmware","Check if update firmaware command");
		//check if message has arrived
		//get data
		//load if to memory
		vTaskDelay(10000 / portTICK_PERIOD_MS);
	}
}


/*
 *  method for creating task for updating firmware
 *  Out:
 *  0 - everything is ok
 *  1 - task creation has failed
 */
int CreateUpdateFirmwareTask(void)
{
	BaseType_t response;

	/*
	 * TASKS for core 0
	 */
	response = xTaskCreatePinnedToCore(
			Task_MQTTRead_UpdateFirmware, //task function
			"UpdateFirmware", //task name
			2048, //stack size
			NULL,//parameters
			5,	//priority (higher is more important)
			&firmwareUpdateTaskHandle, //task handle
			0); //task core

	if (response != pdPASS){
		//TODO somethins is wrong - we should tell someone something
		ESP_LOGE("InitFreeRTOSStructs", "Error : Task_MQTTRead_UpdateFirmware");
		while(1);//quick fix
	}
	return 0;
}
