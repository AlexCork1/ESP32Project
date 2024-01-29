#include "mytasks.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <driver/gpio.h>

#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/idf_additions.h"

#include "SensorTempHum.h"
#include "MQTT_Message.h"
#include "mytasks.h"
#include "esp_log.h"


/*	PRIVATE VARIABLES BEGIN	*/
#define GPIO_PIN_BUTTON GPIO_NUM_25
const uint32_t PERIODIC_TASK_FREQUENCY_MS = 5000;
const uint32_t READ_TEMPERATURE_SENSOR_FREQUENCY_MS = 30000;
SemaphoreHandle_t message_received_sem;

QueueHandle_t jsonQueue;

TaskHandle_t 	sensorUpdateTaskHandle,
				firmwareUpdateTaskHandle,
				mqttPublishTaskHandle,
				sensorPeriodicTaskHandle,
				sensorButtonTaskHandle,
				sensorMQTTTaskHandle;
/*	PRIVATE VARIABLES END	*/


/*	PRIVATE FUNCTIONS PROTOTYPES BEGIN	*/
void Task_MQTTRead_UpdateFirmware(void *params);
void Task_GetTemperatureAndHumidity(void* params);
void Task_PeriodicRead(void *params);
void Task_ReadUserButton(void* params);
void Task_MQTTRead_Command(void *params);
void Task_MQTT_Publish(void* params);
/*	PRIVATE FUNCTIONS PROTOTYPES END	*/

void InitFreeRTOSStructs(void)
{
	jsonQueue = xQueueCreate(5, sizeof(MQTT_MESSAGE));
	message_received_sem = xSemaphoreCreateBinary();

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


	/*
	 * TASKS for core 1
	 */
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

	response = xTaskCreatePinnedToCore(
			Task_ReadUserButton, //task function
			"Sensor_ReadUserButton", //task name
			2048, //stack size
			NULL,//parameters
			3,	//priority (higher is more important)
			&sensorButtonTaskHandle, //task handle
			1); //task core

	if (response != pdPASS){
		//TODO somethins is wrong - we should tell someone something
		ESP_LOGE("InitFreeRTOSStructs", "Error : Task_ReadUserButton");
		while(1);//quick fix
	}

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


	response = xTaskCreatePinnedToCore(
			Task_MQTT_Publish, //task function
			"MQTT_Publish", //task name
			2048, //stack size
			NULL,//parameters
			4,	//priority (higher is more important)
			&mqttPublishTaskHandle, //task handle
			1); //task core

	if (response != pdPASS){
		//TODO somethins is wrong - we should tell someone something
		ESP_LOGE("InitFreeRTOSStructs", "Error : Task_MQTT_Publish");
		while(1);//quick fix
	}
}


/*
 *
 * TASKS
 *
 */
/*
 * tasks runing on core 0
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
 * tasks runing on core 1
 */
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



void Task_MQTT_Publish(void* params)
{
	MQTT_MESSAGE message;
	const uint32_t time_to_wait_ms = 100 / portTICK_PERIOD_MS;
	while(1)
	{
		if(xQueueReceive(jsonQueue, &message, 0) == pdTRUE){
			ESP_LOGI("Task_MQTT_Publish", "Task_MQTT_Publish: %s\n", message.text);
			//todo send to mqtt broker
		}
		vTaskDelay(time_to_wait_ms);
	}
}

