#include <MQTT_GenerateMessage.h>
#include "InitTasks.h"
#include "CommonVariablesAllTasks.h"

#include "UpdateFirmwareTask.h"
#include "ReadUserButtonTask.h"
#include "ReadSensorTask.h"
#include "PeriodicReadTask.h"
#include "MQTTReadTask.h"
#include "MQTTPublishTask.h"

/*
 * how many messages can be stored in queue. This can be changed if there is too many request at the same time
 */
const int QUEUE_SIZE = 5;

void InitFreeRTOSStructs(void)
{
	/*
	 * queue for sending data between all read tasks and publish tasks
	 */
	jsonQueue = xQueueCreate(QUEUE_SIZE, sizeof(MQTT_MESSAGE));

	/*
	 * creating binary semaphores used for signaling
	 */
	message_received_sem = xSemaphoreCreateBinary();
	message_ota_mqtt = xSemaphoreCreateBinary();

	/*
	 * create all tasks (methods are implemented in tasks files)
	 */
	CreateUpdateFirmwareTask();
	CreateReadSensorTask();
	CreatePeriodicTask();
	CreateReadUserButtonTask();
	CreateMQTTReadTask();
	CreateMQTTPublishTask();
}

