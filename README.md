# Weather station

Welcome to our ESP32 project! This modular firmware, designed for the ESP32 module, seamlessly integrates with an external button and utilizes FreeRTOS middleware for efficient task management. 
The system employs three tasks for reading temperature and humidity sensor data: a periodic task, a button-click task, and an MQTT receive task. Each task independently publishes real-time temperature and 
humidity values to an MQTT broker in JSON format, ensuring data integrity and privacy with mutual authentication. The project also supports hassle-free Over-the-Air (OTA) firmware updates.

## Project Structure and Contribution

The code is structured in a modular fashion, emphasizing flexibility and ease of maintenance. Areas for improvement include the implementation of comprehensive tests (both unit and integration) and
 the integration of Espressif's unified provisioning process. Contributions and feedback are welcomed as we collaborate to enhance and refine this ESP32 project.

## Code Structure

The project is organized into several key directories, each serving a specific purpose:

- **`/main`:** Contains the main application code, including the entry point (task) (`main.c`) and creationg of tasks.
   - **`/cert`:** Houses the certificates required for secure connection to MQTT broker, as generated for the MQTTS functionality.
- **`/components`:** Includes reusable components and libraries utilized across the project for improved modularity.
  - **`/rtosTasks`:** Encompasses tasks related to FreeRTOS middleware, crucial for efficient task management. Key files include:
    - `CommonVariablesAllTasks.c`: Defines all "global" variables used by all tasks.
    - `InitTasks.c`: Responsible for call all create task functions and for creation of FreeRTOS middlevare.
    - `MQTTPublishTask.c`: task responsible for publishing MQTT messages to broker. It continuously waits for messages from a queue (data from all other tasks) and publishes them to an MQTT broker.
    - `MQTTReadTask.c`:  task, which is triggered by a semaphore, when MQTT request to read data from a sensor is received. It generates an MQTT message in JSON format using the obtained data, and then sends this message to a queue to publish task. 
    - `PeriodicReadTask.c`: task responsible for periodically reading data from a sensor. It generates an MQTT message in JSON format using the obtained data, and then sends this message to a queue to publish task.
    - `ReadSensorTask.c`: task that implements the periodic task responsible for actual reading temperature and humidity from sensor.
    - `ReadUserButtonTask.c`: task that read external button clicks (polling method, not ISR) and when button press is detected, generates an MQTT message in JSON format using the obtained data, and then sends this message to a queue to publish task.
    - `UpdateFirmwareTask.c`: task responsible for triggering firmware update based on a semaphore signal from MQTT request, initiating the Over-the-Air (OTA) firmware update process.
  - **`/sensor`:** Files related to the temperature and sensor commuincation
  	- `SensorTempHum.c`: Code that works with sensore module. All sensor communication is done here. First, it initializes the sensor module. Then there are two main functions: one does the actual reading of sensor and updates sensor data in a thread-safe manner. Second one retrieves the current sensor data in a thread-safe manner.
  - **`/wifimqtt`:** Code that handles all WiFi and MQTT connections
    - `MQTT_GenerateMessage.c`: Get data from tasks and converts them to JSON format
    - `TriggerType.h`: enum definig all trigger types
    - `WiFiMQTT.c`: function calling ESP api for WiFi and MQTT connections
- **`/sdkconfig`:** Stores the configuration files for the ESP-IDF framework, allowing users to customize WiFi credentials, MQTT broker details, and other parameters.


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
