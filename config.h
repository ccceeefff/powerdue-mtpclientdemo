#ifndef CONFIG_H
#define CONFIG_H

#include <limits.h>

#define CLOCK_TICK_PERIOD pdMS_TO_TICKS(1)  // every 1 millisecond

#define MTP_RATE pdMS_TO_TICKS(10000)       // perform MTP every 10seconds
#define TIMEOUT_TICKS pdMS_TO_TICKS(1000)   // MTP request timeout

#define CLOCK_GPIO_OUT 51

#define WIFI_SSID "PowerDue"
#define WIFI_PASS "powerdue"

#define MTP_PORT 12345

#endif  //CONFIG_H
