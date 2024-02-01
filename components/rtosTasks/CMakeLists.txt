# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS "CommonVariablesAllTasks.c" "InitTasks.c" "MQTTPublishTask.c" "MQTTReadTask.c" "PeriodicReadTask.c" "ReadSensorTask.c" "ReadUserButtonTask.c" "UpdateFirmwareTask.c"
	INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}"
    REQUIRES driver mqtt main sensor wifimqtt esp_http_client esp_https_ota
)
