#include "InitTasks.h"
#include "WiFiMQTT.h"

void app_main(void)
{
	InitWiFi();
	InitMQTT();
	InitFreeRTOSStructs();
}

