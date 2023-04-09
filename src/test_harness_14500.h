
#ifndef __TEST_HARNESS_14500__
#define __TEST_HARNESS_14500__

#include <Arduino.h>
#include "test_harness.h"

#define DUT_PINS_COUNT 16

#define PIN_RST 2
#define PIN_WRITE 3
#define PIN_DATA 4
#define PIN_I3 5
#define PIN_I2 6
#define PIN_I1 7
#define PIN_I0 8
#define PIN_VSS 9
#define PIN_FLAGF 10
#define PIN_FLAGO 11
#define PIN_RTN 12
#define PIN_JMP 13
#define PIN_X2 A0
#define PIN_X1 A1
#define PIN_RR A2
#define PIN_VDD A3

#define INST_NOPO 0x0
#define INST_LD 0x1
#define INST_LDC 0x2
#define INST_OR 0x3
#define INST_ANDC 0x4
#define INST_AND 0x5
#define INST_ORC 0x6
#define INST_XNOR 0x7
#define INST_STO 0x8
#define INST_STOC 0x9
#define INST_IEN 0xa
#define INST_OEN 0xb
#define INST_JMP 0xc
#define INST_RTN 0xd
#define INST_SKZ 0xe
#define INST_NOPF 0xf

#define CLOCK_HI 1
#define CLOCK_LO 2
#define CLOCK_HILO CLOCK_HI + CLOCK_LO

void releaseMC14500();
void powerUpMC14500();
void executeInstructionOnMC14500(byte instruction, byte clockPhases = CLOCK_HILO);
void clockMC14500(byte phases);
bool setIen(byte status);
bool setOen(byte status);

#endif
