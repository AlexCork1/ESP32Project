# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(app-template)

target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/certs/client.crt" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/certs/client.key" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/certs/mosquitto.org.crt" TEXT)

#idf_component_register(SRCS "main/main.c"
                       #INCLUDE_DIRS "main"
                       #REQUIRES rtosTasks sensor wifimqtt)