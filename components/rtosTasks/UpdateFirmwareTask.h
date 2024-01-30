#ifndef MAIN_RTOSTASKS_UPDATEFIRMWARETASK_H_
#define MAIN_RTOSTASKS_UPDATEFIRMWARETASK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern TaskHandle_t firmwareUpdateTaskHandle;

int CreateUpdateFirmwareTask(void);



#endif /* MAIN_RTOSTASKS_UPDATEFIRMWARETASK_H_ */
