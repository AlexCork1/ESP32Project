/*
 * MQTT_Message.h
 *
 *  Created on: 28 Jan 2024
 *      Author: Ales
 */

#ifndef MAIN_MQTT_MESSAGE_H_
#define MAIN_MQTT_MESSAGE_H_

#include <stdio.h>
#include <stdint.h>
#include "SensorData.h"
#include "EventSource.h"

#define MQTT_MESSAGE_MAX_LEN 75
const char* MQTT_FORMAT = "{\"source\":%d,\"data\":{\"temp\": %d,\"hum\":%d}}\0";

typedef struct {
    char text[MQTT_MESSAGE_MAX_LEN];
} MQTT_MESSAGE;

int32_t GeneratePublishMessage(MQTT_MESSAGE *message, TriggerType source, const SensorData *sensorData)
{
	// Use snprintf to format the message based on MQTT_FORMAT
	int32_t response = snprintf(message->text,
								MQTT_MESSAGE_MAX_LEN,
								MQTT_FORMAT,
								(int)source,
								sensorData->temperature,
								sensorData->humidity);

	// Check for snprintf errors
	return (response < 0 ? -1 : 1);
}

#endif /* MAIN_MQTT_MESSAGE_H_ */
