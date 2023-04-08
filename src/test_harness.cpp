#include "test_harness.h"

void setupTestHarness()
{
   pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_RED, LOW);

}

void setLine(byte line, byte level)
{
  pinMode(line, OUTPUT);
  digitalWrite(line, level);
}

byte readLine(byte line)
{
  pinMode(line, INPUT);
  return digitalRead(line);
}

bool expect(byte line, byte value, const char *message)
{
  Serial.print(message);

  if (readLine(line) != value)
  {
    Serial.println(" FAIL");
    return false;
  }

  Serial.println(" OK");
  return true;
}

bool isButtonPressed()
{
  return analogRead(PIN_BUTTON) < 5;
}
