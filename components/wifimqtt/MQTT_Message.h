#ifndef COMPONENTS_WIFIMQTT_MQTT_MESSAGE_H_
#define COMPONENTS_WIFIMQTT_MQTT_MESSAGE_H_

/*
 *
 * Message that is published to MQTT broker.
 * Struct is used to make code more readable
 *
 */

#define MQTT_MESSAGE_MAX_LEN 75

typedef struct {
    char text[MQTT_MESSAGE_MAX_LEN];
} MQTT_MESSAGE;


#endif /* COMPONENTS_WIFIMQTT_MQTT_MESSAGE_H_ */
