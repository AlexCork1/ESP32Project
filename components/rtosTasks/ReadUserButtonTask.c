#include <driver/gpio.h>
#include "MQTT_GenerateMessage.h"
#include <TriggerType.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ReadUserButtonTask.h"
#include "CommonVariablesAllTasks.h"
#include "SensorData.h"
#include "SensorTempHum.h"


TaskHandle_t sensorButtonTaskHandle;

#define GPIO_PIN_BUTTON GPIO_NUM_25

void Task_ReadUserButton(void* params)
{
	const uint32_t frequency_timer_ms = 50 / portTICK_PERIOD_MS;
	const uint32_t debounce_timer_ms = 200 / portTICK_PERIOD_MS;

	gpio_config_t button_config = {
	        .pin_bit_mask = (1ULL<<GPIO_PIN_BUTTON),
	        .mode = GPIO_MODE_INPUT,
	        .intr_type = GPIO_INTR_DISABLE,  // Trigger on any edge (rising or falling)
	        .pull_up_en = GPIO_PULLUP_ENABLE,
	        .pull_down_en = GPIO_PULLDOWN_DISABLE,
	    };
	    gpio_config(&button_config);

	SensorData data;
	MQTT_MESSAGE message;
	uint8_t previousLevel = 1;
	uint8_t newMessageArrived = 0;

	while(1)
	{
		int level = gpio_get_level(GPIO_PIN_BUTTON);
		if (level == 0 && previousLevel == 1)
		{
			Sensor_GetData(&data);
    		ESP_LOGI("Task_ReadUserButton", "Button click : message send to queue");

			GeneratePublishMessage(&message, BUTTON_PRESS, &data);

			//debounce timer
			vTaskDelay(debounce_timer_ms);
			newMessageArrived = 1;
		}

        if (newMessageArrived == 1)
        {
    		//try to send message to queue
    		if(xQueueSend(jsonQueue, &message, 0) == pdTRUE)
    			newMessageArrived = 0;
        }

		vTaskDelay(frequency_timer_ms);
		previousLevel = level;
	}
}

int CreateReadUserButtonTask(void)
{
	BaseType_t response;
	response = xTaskCreatePinnedToCore(
			Task_ReadUserButton, //task function
			"Sensor_ReadUserButton", //task name
			2048, //stack size
			NULL,//parameters
			3,	//priority (higher is more important)
			&sensorButtonTaskHandle, //task handle
			1); //task core

	if (response != pdPASS){
		ESP_LOGE("InitFreeRTOSStructs", "Error : Task_ReadUserButton");
		while(1);//quick fix
	}
	return 0;
}
