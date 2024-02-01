#include "UpdateFirmwareTask.h"
#include "CommonVariablesAllTasks.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_https_ota.h"

TaskHandle_t firmwareUpdateTaskHandle;

/*
 * Task that will update firmware on request
 *
 */
static const char *TAG_OTA = "OTA_TASK";

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
        ESP_LOGI(TAG_OTA, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG_OTA, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG_OTA, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG_OTA, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG_OTA, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG_OTA, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGD(TAG_OTA, "HTTP_EVENT_DISCONNECTED");
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGD(TAG_OTA, "HTTP_REDIRECT");
        break;
    }
    return ESP_OK;
}

void Update()
{
    esp_http_client_config_t config =
    {
        .url = CONFIG_FIRMWARE_UPGRADE_URL
    };
    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };


    ESP_LOGI(TAG_OTA, "Task_MQTTRead_UpdateFirmware : update triggered - url of file:  %s", CONFIG_FIRMWARE_UPGRADE_URL);
    esp_err_t ret = esp_https_ota(&ota_config);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG_OTA, "OTA Succeed, Rebooting...");
        esp_restart();
    } else {
        ESP_LOGE(TAG_OTA, "Firmware upgrade failed");
    }
}
void Task_MQTTRead_UpdateFirmware(void *pvParameter)
{
	while(1)
	{
        ESP_LOGI(TAG_OTA, "Task_MQTTRead_UpdateFirmware : task sleeping");
		// Wait for the semaphore signal indicating a message was received
        if (xSemaphoreTake(message_ota_mqtt, portMAX_DELAY) == pdTRUE)
        {
        	Update();
        }
        ESP_LOGI(TAG_OTA, "Task_MQTTRead_UpdateFirmware : task resuming");
    	vTaskDelay(200);
	}
}


/*
 *  method for creating task for updating firmware
 *  Out:
 *  0 - everything is ok
 *  1 - task creation has failed
 */
int CreateUpdateFirmwareTask(void)
{
	BaseType_t response;

	/*
	 * TASKS for core 0
	 */
	response = xTaskCreatePinnedToCore(
			Task_MQTTRead_UpdateFirmware, //task function
			"UpdateFirmware", //task name
			16384, //stack size
			NULL,//parameters
			6,	//priority (higher is more important)
			&firmwareUpdateTaskHandle, //task handle
			1); //task core

	if (response != pdPASS){
		//TODO somethins is wrong - we should tell someone something
		ESP_LOGE("InitFreeRTOSStructs", "Error : Task_MQTTRead_UpdateFirmware");
		while(1);//quick fix
	}
	return 0;
}
