#include "SensorTempHum.h"
#include <stdint.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/*
 * Code where all reading from temperature sensor is done
 */

/*
 * Private variables
 */
SensorData _local;
SemaphoreHandle_t _sensorDataUpdateSemaphore;
const int32_t SemaphoreWait_MS = 500 / portTICK_PERIOD_MS;

/*
 * Private functions prototypes
 */
int32_t GetRandom(int32_t MIN, int32_t MAX);

void Sensor_Init(uint32_t seed)
{
	_sensorDataUpdateSemaphore = xSemaphoreCreateBinary();
	srand(seed);

	_local.humidity = GetRandom(0, 100);
	_local.temperature = GetRandom(-30, 45);
}

/*
 * This function "reads" new data from temperature sensor
 */
void Sensor_UpdateData()
{
	if( _sensorDataUpdateSemaphore != NULL )
	{
		//get into danger zone
		if (xSemaphoreTake(_sensorDataUpdateSemaphore, SemaphoreWait_MS) == pdTRUE)
		{
			//replace values
			_local.humidity = GetRandom(0, 100); //or something
			_local.temperature = GetRandom(-30, 45); //or something

			//release semaphore
			xSemaphoreGive(_sensorDataUpdateSemaphore);
		}

	}
}
void Sensor_GetData(SensorData *data)
{

	if( _sensorDataUpdateSemaphore != NULL )
	{
		//get into danger zone
		if (xSemaphoreTake(_sensorDataUpdateSemaphore, SemaphoreWait_MS) == pdTRUE)
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

