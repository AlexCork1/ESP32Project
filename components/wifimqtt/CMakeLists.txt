# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS "WiFiMQTT.c" "MQTT_GenerateMessage.c"
	INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}"
    REQUIRES esp_wifi nvs_flash sensor rtosTasks
)
