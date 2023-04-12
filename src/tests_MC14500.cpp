#include "tests_MC14500.h"

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

bool runTest()
{
    Serial.println(F("------------- START -------------"));

  powerUpMC14500();

  Serial.println(F("Power up                       OK"));

  bool result = testIEN() &&
           testOEN() &&
           testLD() &&
           testLDC() &&
           testSTO() &&
           testNOPO() &&
           testORC() &&
           testAND() &&
           testANDC() && 
           testXNOR() &&
           testSTOC() &&
           testJMP() &&
           testRTN() &&
           testSKZ() &&
           testNOPF();

  releaseMC14500();

  Serial.println(F("Release DUT                    OK"));
  Serial.println(F("-------------  END  -------------"));
  Serial.println("");

  return result;
}

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
    // refer to X1. So the instruction is latched on the
    // rising edge of the clock and executed on the falling.

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

bool testLD()
{
    bool result = true;

    setIen(HIGH);
    setOen(HIGH);

    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LD);

    result = result && expect(PIN_RR, HIGH, "testLD.RR1");

    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_LD);

    result = result && expect(PIN_RR, LOW, "testLD.RR2");

    return result;
}

bool testLDC()
{
bool result = true;

    setIen(HIGH);
    setOen(HIGH);

    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LDC);

    result = result && expect(PIN_RR, LOW, "testLDC.RR1");

    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_LDC);

    result = result && expect(PIN_RR, HIGH, "testLDC.RR2");

    return result;
}

bool testOEN()
{
    bool result = true;

    setIen(HIGH);
    setOen(LOW);

    executeInstructionOnMC14500(INST_STO, CLOCK_HI);

    result = result && expect(PIN_WRITE, LOW, "testOEN.WRITE1");

    clockMC14500(CLOCK_LO);

    result = result && expect(PIN_WRITE, LOW, "testOEN.WRITE2");

    return result;
}

bool testSTO()
{
    bool result = true;

    setIen(HIGH);
    setOen(HIGH);

    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_STO, CLOCK_HI);

    result = result &&
             expect(PIN_DATA, HIGH, "testSTOH.DATA") &&
             expect(PIN_WRITE, HIGH, "testSTOH.WRITEH") &&
             expect(PIN_RR, HIGH, "testSTOH.RR");

    clockMC14500(CLOCK_LO);

    result = result &&
             expect(PIN_DATA, HIGH, "testSTOH.DATA") &&
             expect(PIN_WRITE, LOW, "testSTOH.WRITEL") &&
             expect(PIN_RR, HIGH, "testSTOH.RR");

    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_STO, CLOCK_HI);

    result = result &&
             expect(PIN_DATA, LOW, "testSTOL.DATA") &&
             expect(PIN_WRITE, HIGH, "testSTOL.WRITEH") &&
             expect(PIN_RR, LOW, "testSTOL.RR");

    clockMC14500(CLOCK_LO);

    result = result &&
             expect(PIN_DATA, LOW, "testSTOL.DATA") &&
             expect(PIN_WRITE, LOW, "testSTOL.WRITEL") &&
             expect(PIN_RR, LOW, "testSTOH.RR");

    return result;
}

bool testIEN()
{
    setIen(LOW);
    setOen(HIGH);

    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LD);

    return expect(PIN_RR, LOW, "testIEN.RR");
}

bool testNOPO()
{
    setIen(HIGH);
    setOen(HIGH);

    executeInstructionOnMC14500(INST_NOPO);

    return expect(PIN_FLAGO, HIGH, "testNOPO.FLAGO");
}

bool testORC()
{
    bool result = true;

    setIen(HIGH);
    setOen(HIGH);

    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_ORC);

    result = result && expect(PIN_RR, HIGH, "testORC.RR1");

    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_ORC);

    result = result && expect(PIN_RR, HIGH, "testORC.RR2");

    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LD);
    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_ORC);

    result = result && expect(PIN_RR, HIGH, "testORC.RR3");

    return result;
}

bool testAND()
{
    bool result = true;

    setIen(HIGH);
    setOen(HIGH);

    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_AND);

    result = result && expect(PIN_RR, HIGH, "testAND.RR1");

    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_AND);

    result = result && expect(PIN_RR, LOW, "testAND.RR2");

    return result;
}

bool testANDC()
{
    bool result = true;

    setIen(HIGH);
    setOen(HIGH);

    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LD);
    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_ANDC);
    result = result && expect(PIN_RR, LOW, "testANDC.RR1");

    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_LD);
    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_ANDC);
    result = result && expect(PIN_RR, LOW, "testANDC.RR2");

    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LD);
    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_ANDC);

    result = result && expect(PIN_RR, HIGH, "testANDC.RR3");

    return result;
}

bool testXNOR()
{
    bool result = true;

    setIen(HIGH);
    setOen(HIGH);

    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_XNOR);

    result = result && expect(PIN_RR, HIGH, "testXNOR.RR1");

    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_XNOR);

    result = result && expect(PIN_RR, HIGH, "testXNOR.RR2");

    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_LD);
    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_XNOR);

    result = result && expect(PIN_RR, LOW, "testXNOR.RR3");

    return result;
}
bool testSTOC()
{
    bool result = true;

    setIen(HIGH);
    setOen(HIGH);

    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_STOC, CLOCK_HI);

    result = result &&
             expect(PIN_DATA, LOW, "testSTOCH.DATA") &&
             expect(PIN_WRITE, HIGH, "testSTOCH.WRITEH") &&
             expect(PIN_RR, HIGH, "testSTOCH.RR");

    clockMC14500(CLOCK_LO);

    result = result &&
             expect(PIN_DATA, LOW, "testSTOCH.DATA") &&
             expect(PIN_WRITE, LOW, "testSTOCH.WRITEL") &&
             expect(PIN_RR, HIGH, "testSTOCH.RR");

    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_STOC, CLOCK_HI);

    result = result &&
             expect(PIN_DATA, HIGH, "testSTOCL.DATA") &&
             expect(PIN_WRITE, HIGH, "testSTOCL.WRITEH") &&
             expect(PIN_RR, LOW, "testSTOCL.RR");

    clockMC14500(CLOCK_LO);

    result = result &&
             expect(PIN_DATA, HIGH, "testSTOCL.DATA") &&
             expect(PIN_WRITE, LOW, "testSTOCL.WRITEL") &&
             expect(PIN_RR, LOW, "testSTOCL.RR");

    return result;
}

bool testJMP()
{
    bool result = true;

    setIen(HIGH);
    setOen(HIGH);

    executeInstructionOnMC14500(INST_JMP, CLOCK_HI);

    result = result && expect(PIN_JMP, HIGH, "testJMP.JMP1");

    clockMC14500(CLOCK_LO);

    result = result && expect(PIN_JMP, HIGH, "testJMP.JMP2");

    return result;
}

bool testRTN()
{
    bool result = true;

    setIen(HIGH);
    setOen(HIGH);

    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_RTN);

    result = result && expect(PIN_RTN, HIGH, "testRTN.RTN");

    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_RTN);
    executeInstructionOnMC14500(INST_LDC);

    result = result && expect(PIN_RR, LOW, "testRTN.RR");

    return result;
}

bool testSKZ()
{
    bool result = true;

    setIen(HIGH);
    setOen(HIGH);

    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_SKZ);
    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LD);

    result = result && expect(PIN_RR, HIGH, "testSKZ.RR1");

    setLine(PIN_DATA, LOW);
    executeInstructionOnMC14500(INST_LD);
    executeInstructionOnMC14500(INST_SKZ);
    setLine(PIN_DATA, HIGH);
    executeInstructionOnMC14500(INST_LD);

    result = result && expect(PIN_RR, LOW, "testSKZ.RR2");

    return result;
}

bool testNOPF()
{
    setIen(HIGH);
    setOen(HIGH);

    executeInstructionOnMC14500(INST_NOPF);

    return expect(PIN_FLAGF, HIGH, "testNOPF.FLAGF");
}
