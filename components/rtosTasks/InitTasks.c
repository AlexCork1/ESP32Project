#include <MQTT_GenerateMessage.h>
#include "InitTasks.h"
#include "CommonVariablesAllTasks.h"

#include "UpdateFirmwareTask.h"
#include "ReadUserButtonTask.h"
#include "ReadSensorTask.h"
#include "PeriodicReadTask.h"
#include "MQTTReadTask.h"
#include "MQTTPublishTask.h"

void InitFreeRTOSStructs(void)
{
	jsonQueue = xQueueCreate(5, sizeof(MQTT_MESSAGE));
	message_received_sem = xSemaphoreCreateBinary();

	CreateUpdateFirmwareTask();
	CreateReadSensorTask();
	CreatePeriodicTask();
	CreateReadUserButtonTask();
	CreateMQTTReadTask();
	CreateMQTTPublishTask();
}

