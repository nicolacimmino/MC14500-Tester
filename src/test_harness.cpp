#include "test_harness.h"

char printBuffer[PRINT_BUFFER_SIZE];
uint8_t failures = 0;
uint8_t asserts = 0;

void setupTestHarness()
{
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, LOW);
    failures = 0;
    asserts = 0;
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

void expect(byte line, byte value, const char *message)
{
    bool result = readLine(line) == value;

    sprintf(printBuffer, "%-30s %s\r\n", message, (result ? "OK" : "FAIL"));
    Serial.print(printBuffer);

    asserts++;
    if (!result)
    {
        failures++;
    }
}

bool isButtonPressed()
{
    return analogRead(PIN_BUTTON) < 5;
}
