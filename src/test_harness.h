
#ifndef __TEST_HARNESS_H__
#define __TEST_HARNESS_H__

#include <Arduino.h>

#define PIN_BUTTON A6
#define PIN_LED_RED A5
#define PIN_LED_GREEN A4

#define PRINT_BUFFER_SIZE 80

bool isButtonPressed();
void setupTestHarness();
void setLine(byte line, byte level);
byte readLine(byte line);
bool expect(byte line, byte value, const char *message);

#endif