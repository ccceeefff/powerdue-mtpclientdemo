#ifndef _POWERDUE_H_
#define _POWERDUE_H_

#define LED_RGB_RED 6
#define LED_RGB_GREEN 7
#define LED_RGB_BLUE 8
#define PD_RED 1
#define PD_GREEN 2
#define PD_BLUE 3
#define PD_YELLOW 4
#define PD_PURPLE 5
#define PD_TIEL 6
#define PD_WHITE 7
#define PD_OFF 8

#define pd_rgb_led_init() { \
  pinMode(LED_RGB_RED, OUTPUT); \
  pinMode(LED_RGB_GREEN, OUTPUT); \
  pinMode(LED_RGB_BLUE, OUTPUT); \
  digitalWrite(LED_RGB_RED, LOW); \
  digitalWrite(LED_RGB_GREEN, LOW); \
  digitalWrite(LED_RGB_BLUE, LOW); \
}

#define pd_rgb_led(color) { \
  if(color == PD_RED || color == PD_YELLOW || color == PD_PURPLE || color == PD_WHITE){ \
    digitalWrite(LED_RGB_RED, HIGH); \
  }else{ \
    digitalWrite(LED_RGB_RED, LOW); \
  } \
  if(color == PD_GREEN || color == PD_YELLOW || color == PD_TIEL || color == PD_WHITE){ \
    digitalWrite(LED_RGB_GREEN, HIGH); \
  }else{ \
    digitalWrite(LED_RGB_GREEN, LOW); \
  } \
  if(color == PD_BLUE || color == PD_PURPLE || color == PD_TIEL || color == PD_WHITE){ \
    digitalWrite(LED_RGB_BLUE, HIGH); \
  }else{ \
    digitalWrite(LED_RGB_BLUE, LOW); \
  } \
}

#endif
