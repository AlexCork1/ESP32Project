/*
 * SensorData.h
 *
 *  Created on: 28 Jan 2024
 *      Author: Ales
 */

#ifndef MAIN_SENSORDATA_H_
#define MAIN_SENSORDATA_H_

#include <stdint.h>

/*
 * struct used to represent data from sensor
 */

typedef struct{
    int32_t temperature;
    int32_t humidity;
}SensorData;

#endif /* MAIN_SENSORDATA_H_ */
