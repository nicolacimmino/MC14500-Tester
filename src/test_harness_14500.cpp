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

  delay(10);
  digitalWrite(PIN_RST, HIGH);
  delay(10);
  digitalWrite(PIN_RST, LOW);
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
  executeInstructionOnMC14500(INST_IEN, CLOCK_HILO);
}

bool setOen(byte status)
{
  setLine(PIN_DATA, status);
  executeInstructionOnMC14500(INST_OEN, CLOCK_HILO);
}
