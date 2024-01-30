/*
 * SensorTempHum.h
 *
 *  Created on: 28 Jan 2024
 *      Author: Ales
 */

#ifndef MAIN_SENSORTEMPHUM_H_
#define MAIN_SENSORTEMPHUM_H_

#include "SensorData.h"

/*
 *
 */

void Sensor_Init(uint32_t randomSeed);
void Sensor_UpdateData();
void Sensor_GetData(SensorData *data);



#endif /* MAIN_SENSORTEMPHUM_H_ */
