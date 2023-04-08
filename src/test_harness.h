
#ifndef __TEST_HARNESS_H__
#define __TEST_HARNESS_H__

#define PIN_BUTTON A6
#define PIN_LED_RED A5
#define PIN_LED_GREEN A4

#include <Arduino.h>

bool isButtonPressed();
void setupTestHarness();
void setLine(byte line, byte level);
byte readLine(byte line);
bool expect(byte line, byte value, const char *message);

#endif