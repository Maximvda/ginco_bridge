#pragma once

typedef enum
{
    EVENT_UNUSED_EVENT,
    EVENT_GPIO_TOGGLE,
    EVENT_CAN_TRANSMIT,
    EVENT_CAN_RECEIVED,
    EVENT_WRONG_SSID,
    EVENT_START_STA_INTERFACE,
    EVENT_SET_SSID_PASS,
} eEventType;
