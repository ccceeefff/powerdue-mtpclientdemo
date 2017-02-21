#include <FreeRTOS_ARM.h>

#include <IPAddress.h>

#include "PowerDue.h"
#include "config.h"
#include "debug.h"
#include "mtp.h"
#include "wifi_util.h"

// A UDP instance to let us send and receive packets over UDP
WiFiEspUDP Udp;
IPAddress MTP_Server(10, 230, 4, 4);

QueueHandle_t xMtpTriggerQueue;
volatile uint32_t clockTicks = 0;

/*
 * Clock Functions
 */

inline void initPins(){
  pinMode(CLOCK_GPIO_OUT, OUTPUT);
}

inline void togglePin(int state){
  digitalWrite(CLOCK_GPIO_OUT, state);
}

void vClockCallback( TimerHandle_t pxTimer ){
  clockTicks++;
  // assumes we're running at 1ms ticks
  // every 10 ticks, toggle GPIO high
  if(clockTicks % 10 == 0){
    togglePin(HIGH);
  }
  // every on the 11th tick, toggle GPIO low
  if(clockTicks % 10 == 1){
    togglePin(LOW);
  }
}

/*
 * MTP Functions
 */

void mtp_get_time(mtp_t *t){
  t->tod = clockTicks;  // number of millisecond ticks that have passed since epoch
  t->frac = 0;          // fraction of a millisecond our timer has made
}

void mtp_compute(mtp_t *t0, mtp_t *t1, mtp_t *t2, mtp_t *t3){
  float f0 = mtp_to_float(t0);
  float f1 = mtp_to_float(t1);
  float f2 = mtp_to_float(t2);
  float f3 = mtp_to_float(t3);

  SerialUSB.print("t0: ");
  SerialUSB.print(f0);
  SerialUSB.print(", t1: ");
  SerialUSB.print(f1);
  SerialUSB.print(", t2: ");
  SerialUSB.print(f2);
  SerialUSB.print(", t3: ");
  SerialUSB.println(f3);

  // compute delay and offset
  // adjust your local clock
}

void mtp_run_loop(){
  int recvPacketSize;
  uint8_t recvBuffer[MTP_RESPONSE_SIZE];
  uint8_t writeBuffer[MTP_REQUEST_SIZE];
  mtp_t t0, t0_ret, t1, t2, t3;

  // send packet
  Udp.beginPacket(MTP_Server, MTP_PORT);
  mtp_get_time(&t0);
  mtp_time_to_buffer(&t0, writeBuffer);
  Udp.write(writeBuffer, MTP_REQUEST_SIZE);
  Udp.endPacket();

  // wait and parse with timeout
  TickType_t tStart = xTaskGetTickCount();
  while(1){
    if(Udp.parsePacket()){
      mtp_get_time(&t3);
      if(Udp.remoteIP() == MTP_Server){
        Udp.read(recvBuffer, MTP_RESPONSE_SIZE);
        // check that first four bytes are the same as t0
        mtp_buffer_to_time(&recvBuffer[0], &t0_ret);
        if(t0.tod == t0_ret.tod){
          // this is a response to the packet we just sent out
          // read the t1 and t2
          mtp_buffer_to_time(&recvBuffer[8], &t1);
          mtp_buffer_to_time(&recvBuffer[16], &t2);

          // compute offset and delay
          mtp_compute(&t0_ret, &t1, &t2, &t3);
          return;
        }
      }
      Udp.flush();
    } else {
      // check for timeout
      if(xTaskGetTickCount() - tStart > TIMEOUT_TICKS){
        // took too long, give up
        return; 
      } else {
        // wait for a bit
        vTaskDelay(pdMS_TO_TICKS(10));  
      }
    }
  }
}

void xWiFiLoop(void *arg){
  // indicates that we've started the MTP client
  pd_rgb_led(PD_BLUE);  
  Udp.begin(MTP_PORT);
  uint8_t message;
  // main wifi loop
  while(1){
    // checks a queue if its time to perform an MTP request
    if(xQueueReceive(xMtpTriggerQueue, &message, 0)){
      pd_rgb_led(PD_GREEN);
      mtp_run_loop();
      pd_rgb_led(PD_BLUE);
    }
    taskYIELD();
  } 
  pd_rgb_led(PD_RED);
  Udp.stop();
}

/* MTP Timer callback thread to trigger an MTP request on the WiFi Task */
void vMtpRequestCallback( TimerHandle_t pxTimer ){
  // send a message on the wifi queue to test the connection
  uint8_t check = 1;
  xQueueSendToBack(xMtpTriggerQueue, &check, 0); // no need to wait for this to finish
}

void setup() {
  SerialUSB.begin(0);

  // wait for SerialUSB open
  while(!SerialUSB);

  pd_rgb_led_init();
  initPins();

  pd_rgb_led(PD_RED);
  
  // establish wifi connection
  setupWiFi(WIFI_SSID, WIFI_PASS, WIFI_BAUD);
  printWifiStatus(SerialUSB);
  
  xMtpTriggerQueue = xQueueCreate(2, sizeof(uint8_t));
 
  TimerHandle_t xClockTimer = xTimerCreate("Clock", CLOCK_TICK_PERIOD, pdTRUE, 0, vClockCallback);
  xTimerStart(xClockTimer, portMAX_DELAY);

  TimerHandle_t xMtpTimer = xTimerCreate("MTP", MTP_RATE, pdTRUE, 0, vMtpRequestCallback);
  xTimerStart(xMtpTimer, portMAX_DELAY);

  xTaskCreate(xWiFiLoop, NULL, configMINIMAL_STACK_SIZE*4, NULL, 1, NULL);
  
  vTaskStartScheduler();
}

void loop() {
  // not used in FreeRTOS
}
