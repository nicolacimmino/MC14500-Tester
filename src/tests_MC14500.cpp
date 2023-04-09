#include "tests_MC14500.h"

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
