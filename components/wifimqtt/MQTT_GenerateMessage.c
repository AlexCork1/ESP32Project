#include "MQTT_GenerateMessage.h"


/*
 * JSON format for message
 */
const char* MQTT_FORMAT = "{\"source\":%d,\"data\":{\"temp\": %d,\"hum\":%d}}\0";


/*
 * Method GeneratePublishMessage: joins trigger type and sensor data to one json format buffer. It gets called from all tasks that read temperature sensor
 * In parameters:
 * 	- MQTT_MESSAGE *message : reference to MQTT_Message type (esp32 uses this notation - it returns "data" by reference, not by return)
 * 	- TriggerType source: based on task who called it
 * 	- const SensorData *sensorData: data from temperature sensor
 *
 * Out:
 * 	- Error check: if snprintf was unsuccessful, method return -1, otherwise 0
 */
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
