#include "MQTT_GenerateMessage.h"


const char* MQTT_FORMAT = "{\"source\":%d,\"data\":{\"temp\": %d,\"hum\":%d}}\0";

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
