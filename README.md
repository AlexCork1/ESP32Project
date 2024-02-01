# Instructions for Using the Weather Sensor

Welcome to our ESP32 project! This modular firmware, designed for the ESP32 module, seamlessly integrates with an external button and utilizes FreeRTOS middleware for efficient task management. 
The system employs three tasks for reading temperature and humidity sensor data: a periodic task, a button-click task, and an MQTT receive task. Each task independently publishes real-time temperature and 
humidity values to an MQTT broker in JSON format, ensuring data integrity and privacy with mutual authentication. The project also supports hassle-free Over-the-Air (OTA) firmware updates.

## Project Structure and Contribution

The code is structured in a modular fashion, emphasizing flexibility and ease of maintenance. Areas for improvement include the implementation of comprehensive tests (both unit and integration) and
 the integration of Espressif's unified provisioning process. Contributions and feedback are welcomed as we collaborate to enhance and refine this ESP32 project.

## Configuration

### Change WiFi Credentials

Modify WiFi credentials (SSID and password) in `sdkconfig` under My Configuration -> "WiFi SSID" and/or "WiFi Password" fields. Rebuild the project and run to apply changes.

### Change MQTT Broker Information

Configure the MQTT broker in `sdkconfig` under My Configuration -> "Broker URL," "MQTT topic request," and/or "MQTT message request" (for receiving the latest reading). Modify "MQTT topic publish" 
to set the topic used by a task to publish messages to the MQTT broker.

### Create Certificate for Secure Connection to MQTT Broker

Support secure connections to the MQTT broker by generating a client certificate for MQTTS. Follow the provided steps for certificate creation and placement in the `/main/cert` folder.

## Button Connection

Connect an external button to ground and pin D25 (modifiable in `ReadUserButtonTask.c`). This button is used for button handling in the task.

## Firmware Update

The project supports Over-the-Air (OTA) firmware updates triggered by an MQTT message sent to the topic "/assignment/update." Customize the update trigger topic in `sdkconfig` under 
My Configuration -> MQTT trigger update topic. Specify the location of the new firmware in `sdkconfig` under My Configuration -> URL address with the update file name. Be aware that 
only non-secure HTTP connections are supported for OTA updates. Exercise caution in security-sensitive environments.

### Firmware Output Examples
- Current firmware output: Message: {"source":0,"data":{"temp": 18,"hum":38}}
- Updated firmware output: Message: {"data source":0,"data":{"temp": 44,"hum":16}}
