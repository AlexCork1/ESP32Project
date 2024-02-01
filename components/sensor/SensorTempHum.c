#include "SensorTempHum.h"
#include <stdint.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"

/*
 * Code where all reading from temperature sensor is done
 * Basic idea is, that only one task reads from sensor and stores is locally, while other always get the latest copy of data
 */

/*
 * Private variables
 */
static SensorData _local;
static SemaphoreHandle_t _sensorDataUpdateSemaphore;
const int32_t SemaphoreWait_MS = 500 / portTICK_PERIOD_MS;

/*
 * Private functions prototypes
 */
static void SetData(int32_t temp, int32_t hum);
int32_t GetRandom(int32_t MIN, int32_t MAX);

void Sensor_Init(uint32_t seed)
{
	_sensorDataUpdateSemaphore = xSemaphoreCreateMutex();
	if(_sensorDataUpdateSemaphore == NULL)
	{
        ESP_LOGE("SENSOR DATA", "Semaphore create failed");
	}
	srand(seed);

	SetData(GetRandom(-30, 45), GetRandom(0, 100));
}

/*
 * This function "reads" new data from temperature sensor
 */
static void SetData(int32_t temp, int32_t hum)
{
	if( _sensorDataUpdateSemaphore != NULL )
	{
		//get into danger zone
		if (xSemaphoreTake(_sensorDataUpdateSemaphore, portMAX_DELAY) == pdTRUE)
		{
			//replace "global" values
			_local.humidity = hum; //or something
			_local.temperature = temp;

			//release semaphore
			xSemaphoreGive(_sensorDataUpdateSemaphore);

		}
	}
}
void Sensor_UpdateData()
{
	int32_t temp = GetRandom(-30, 45);
	int32_t hum = GetRandom(0, 100);

	SetData(temp, hum);
}
void Sensor_GetData(SensorData *data)
{

	if( _sensorDataUpdateSemaphore != NULL )
	{
		//get into danger zone
		if (xSemaphoreTake(_sensorDataUpdateSemaphore, portMAX_DELAY) == pdTRUE)
		{
			//overwrite values
			data->humidity = _local.humidity;
			data->temperature = _local.temperature;

			//release semaphore
			xSemaphoreGive(_sensorDataUpdateSemaphore);
		}
	}
}

/*
 * just for the purpose of assignment, we used random values
 */
int32_t GetRandom(int32_t MIN, int32_t MAX){ return rand() % (MAX - MIN + 1) + MIN; }

