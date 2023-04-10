#include "test_harness_14500.h"

byte dut_pins[] = {
    PIN_RST,
    PIN_WRITE,
    PIN_DATA,
    PIN_I3,
    PIN_I2,
    PIN_I1,
    PIN_I0,
    PIN_VSS,
    PIN_FLAGF,
    PIN_FLAGO,
    PIN_RTN,
    PIN_JMP,
    PIN_X2,
    PIN_X1,
    PIN_RR,
    PIN_VDD,
};

void releaseMC14500()
{
    for (uint8_t ix = 0; ix < DUT_PINS_COUNT; ix++)
    {
        pinMode(dut_pins[ix], INPUT);
    }
}

void powerUpMC14500()
{
    pinMode(PIN_VDD, OUTPUT);
    pinMode(PIN_VSS, OUTPUT);
    pinMode(PIN_RST, OUTPUT);

    digitalWrite(PIN_VDD, HIGH);
    digitalWrite(PIN_VSS, LOW);

    delay(100);
    
    delay(10);
    digitalWrite(PIN_RST, HIGH);
    delay(10);
    digitalWrite(PIN_RST, LOW);

    delay(100);
}

void executeInstructionOnMC14500(byte instruction, byte clockPhases)
{
    pinMode(PIN_I0, OUTPUT);
    pinMode(PIN_I1, OUTPUT);
    pinMode(PIN_I2, OUTPUT);
    pinMode(PIN_I3, OUTPUT);

    digitalWrite(PIN_I0, instruction & 0x1);
    digitalWrite(PIN_I1, (instruction >> 1) & 0x1);
    digitalWrite(PIN_I2, (instruction >> 2) & 0x1);
    digitalWrite(PIN_I3, (instruction >> 3) & 0x1);

    clockMC14500(clockPhases);
}

void clockMC14500(byte phases)
{
    // Note! Because we feed the clock on X2 the phase
    // is opposite of what you see on datasheets which
    // refer to X1. So the instruction is latched on
    // rising edge of the clockand executed on the falling.
    
    pinMode(PIN_X2, OUTPUT);

    if (phases & CLOCK_HI)
    {
        digitalWrite(PIN_X2, HIGH);
    }

    if (phases & CLOCK_LO)
    {
        digitalWrite(PIN_X2, LOW);
    }
}

bool setIen(byte status)
{
    setLine(PIN_DATA, status);
    executeInstructionOnMC14500(INST_IEN);
}

bool setOen(byte status)
{
    setLine(PIN_DATA, status);
    executeInstructionOnMC14500(INST_OEN);
}
