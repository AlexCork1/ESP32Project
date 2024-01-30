#ifndef MAIN_TRIGGERTYPE_H_
#define MAIN_TRIGGERTYPE_H_


/*
 * In tasks program can have different triggers for reading data from temperature sensor.
 */

typedef enum{
    PERIODIC_TIMER = 0,
    BUTTON_PRESS = 1,
    CLOUD_REQUEST = 2
} TriggerType;



#endif /* MAIN_TRIGGERTYPE_H_ */
