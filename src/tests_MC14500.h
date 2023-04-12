
#ifndef __TESTS_MC14500__
#define __TESTS_MC14500__

#include <Arduino.h>
#include "test_harness.h"

#define DUT_PINS_COUNT 16

#define PIN_RST DUT16_PIN_1
#define PIN_WRITE DUT16_PIN_2
#define PIN_DATA DUT16_PIN_3
#define PIN_I3 DUT16_PIN_4
#define PIN_I2 DUT16_PIN_5
#define PIN_I1 DUT16_PIN_6
#define PIN_I0 DUT16_PIN_7
#define PIN_VSS DUT16_PIN_8
#define PIN_FLAGF DUT16_PIN_9
#define PIN_FLAGO DUT16_PIN_10
#define PIN_RTN DUT16_PIN_11
#define PIN_JMP DUT16_PIN_12
#define PIN_X2 DUT16_PIN_13
#define PIN_X1 DUT16_PIN_14
#define PIN_RR DUT16_PIN_15
#define PIN_VDD DUT16_PIN_16

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

bool runTest();

void releaseMC14500();
void powerUpMC14500();
void executeInstructionOnMC14500(byte instruction, byte clockPhases = CLOCK_HILO);
void clockMC14500(byte phases);
bool setIen(byte status);
bool setOen(byte status);

bool testLD();
bool testLDC();
bool testOEN();
bool testSTO();
bool testIEN();
bool testNOPO();
bool testORC();
bool testAND();
bool testANDC();
bool testXNOR();
bool testSTOC();
bool testJMP();
bool testRTN();
bool testSKZ();
bool testNOPF();
bool testX1X2();

#endif