# Instructions for using weather sensor

## Change WiFi credentials
To change WiFi credentials, SSID and Password (if needed), open file sdkconfig, and find in the list option "My Configuration".
There you can change WiFi SSID field and WiFi Password field. Rebuild project and run it to make a change.<br>

## Change MQTT brokerinformation
To change MQTT broker information, open file sdkconfig, and find in the list option "My Configuration". 
There you can change Broker URL, MQTT topic request and message that must be send to task, to get a reading from sensor.
MQTT topic publish is a topic used by another task for publishing data to MQTT broker.

## Create certificate for secure connection to MQTT broker
Generate client certificate for mqtts:
- openssl genrsa -out client.key
- openssl req -out client.csr -key client.key -new<br >
- Go to website https://test.mosquitto.org/ssl/index.php and copy the content of file client.csr in there(save downloaded file in the same folder)<br />

Get server certificate from page https://test.mosquitto.org/ssl/mosquitto.org.crt and save it to folder keys<br />

Copy files client.crt, client.key and mosquitto.org.crt to folder /main/cert<br />

## Button connection
An extern button is used for button requests. It must be connected to ground and pin D25 (this can be changed in file ReadUserButtonTask.c where define must be updated. <br />

## Firmware update
This project supports Over-the-Air (OTA) firmware updates triggered by a MQTT message send to topic "/assignment/update" (data is not important) 
(can be modified in sdkconfig->My Configuration->MQTT trigger update topic). Location of the new, update firmware,
can also be customized in the sdkconfig->My Configuration->URL address with update file name added.<br >

Currenlty on server there is similar, but different firmware to test this functionality:
- Output of current firmware: Message: {"source":0,"data":{"temp": 18,"hum":38}}<br />
- Output of updated firmware: Message: {"data source":0,"data":{"temp": 44,"hum":16}}

It's important to note that only non-secure HTTP connection is supported for OTA updates.Please exercise caution when using this feature in environments where security is a concern. 