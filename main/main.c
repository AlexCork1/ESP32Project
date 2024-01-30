/*
 *
 *  "Entry" point of program
 *  It does three things:
 *  	- connects to WiFi AP
 *  	- connects to MQTT broker
 *  	- creates all tasks (and queues, semaphors, ...)
 *
 */
#include "InitTasks.h"
#include "WiFiMQTT.h"

void app_main(void)
{
	InitWiFi();
	InitMQTT();
	InitFreeRTOSStructs();
}

