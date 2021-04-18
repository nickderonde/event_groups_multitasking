#include <Arduino.h>

// evtgrp.ino 
// MIT License (see file LICENSE)

//event based Blinking test for FreeRTOS ont the ESP32

// LED is active high
#define GPIO_LED1     26
#define GPIO_LED2     25

#define EVBLK2        0b0001
#define EVBLK3        0b0010
#define EVALL         0b0011

static EventGroupHandle_t hevt;

void blink2(void *arg) {

  for (;;) {
    // Call blocks until EVBLK2 bit set,
    // and same bit is cleared upon return
    xEventGroupWaitBits(
      hevt,
      EVBLK2,
      pdTRUE,
      pdFALSE,
      portMAX_DELAY
    );
    // Blink 2 times
    for ( int x=0; x < 6; ++x ) {
      digitalWrite(GPIO_LED1,HIGH);
      delay(250);
      digitalWrite(GPIO_LED1,LOW);
      delay(250);
    }
  }
}

void blink3(void *arg) {

  for (;;) {
    // Call blocks until EVBLK3 bit set,
    // and same bit is cleared upon return
    xEventGroupWaitBits(
      hevt,
      EVBLK3,
      pdTRUE,
      pdFALSE,
      portMAX_DELAY
    );
    // Blink 3 times
    for ( int x=0; x < 3; ++x ) {
      digitalWrite(GPIO_LED2,HIGH);
      delay(100);
      digitalWrite(GPIO_LED2,LOW);
      delay(100);
    }
  }
}

void setup() {
  int app_cpu = xPortGetCoreID();
  BaseType_t rc;

  pinMode(GPIO_LED1,OUTPUT);
  pinMode(GPIO_LED2,OUTPUT);
  digitalWrite(GPIO_LED1,LOW);
  digitalWrite(GPIO_LED2,LOW);

  delay(2000);

  hevt = xEventGroupCreate();
  assert(hevt);

  rc = xTaskCreatePinnedToCore(
    blink2,   // func
    "blink2", // name
    1024,     // stack bytes
    nullptr,  // arg ptr
    1,        // priority
    nullptr,  // ptr to task handle
    app_cpu   // cpu#
  );
  assert(rc == pdPASS);

  rc = xTaskCreatePinnedToCore(
    blink3,   // func
    "blink3", // name
    1024,     // stack bytes
    nullptr,  // arg ptr
    1,        // priority
    nullptr,  // ptr to task handle
    app_cpu   // cpu#
  );
  assert(rc == pdPASS);
}

void loop() {

  delay(5000);
  xEventGroupSetBits(
    hevt,
    EVBLK2|EVBLK3
  );
}

// End evtgrp.ino
