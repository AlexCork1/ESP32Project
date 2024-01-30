#ifndef MAIN_MQTT_MESSAGE_H_
#define MAIN_MQTT_MESSAGE_H_

#include <stdio.h>
#include <stdint.h>
#include "TriggerType.h"
#include "SensorData.h"
#include "MQTT_Message.h"


/*
 * This method takes in all parameters and generates MQTT publish message based on format
 */
int32_t GeneratePublishMessage(MQTT_MESSAGE *message, TriggerType source, const SensorData *sensorData);


#endif /* MAIN_MQTT_MESSAGE_H_ */
