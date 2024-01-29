/*
 * SensorTempHum.c
 *
 *  Created on: 28 Jan 2024
 *      Author: Ales
 */

#include "SensorTempHum.h"
#include <stdint.h>
#include <stdlib.h>

/*
 * Private variables
 */
SensorData local;

/*
 * Private functions prototypes
 */
int32_t GetRandom(int32_t MIN, int32_t MAX);

void Sensor_Init()
{
	srand(5);
	local.humidity = GetRandom(0, 100);
	local.temperature = GetRandom(-30, 45);
}

void Sensor_UpdateData()
{
	//acquire semaphore
	//TODO

	//replace values
	local.humidity = GetRandom(0, 100); //or something
	local.temperature = GetRandom(-30, 45); //or something

	//release semaphore
	//TODO
}
void Sensor_GetData(SensorData *data)
{
	//acquire semaphore
	//TODO

	//overwrite values
	data->humidity = local.humidity;
	data->temperature = local.temperature;

	//release semaphore
	//TODO
}

/*
 * helper functions
 */
int32_t GetRandom(int32_t MIN, int32_t MAX)
{
	return rand() % (MAX - MIN + 1) + MIN;
}

