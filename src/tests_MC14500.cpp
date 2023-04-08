#include "tests_MC14500.h"



bool testLD()
{
  setIen(HIGH);
  setOen(HIGH);

  setLine(PIN_DATA, HIGH);
  executeInstructionOnMC14500(INST_LD, CLOCK_HILO);

  return expect(PIN_RR, HIGH, "testLD.RR");
}

bool testLDC()
{
  setIen(HIGH);
  setOen(HIGH);

  setLine(PIN_DATA, HIGH);
  executeInstructionOnMC14500(INST_LDC, CLOCK_HILO);

  return expect(PIN_RR, LOW, "testLDC.RR");
}

bool testOEN()
{
  setIen(HIGH);
  setOen(LOW);

  setLine(PIN_DATA, HIGH);
  executeInstructionOnMC14500(INST_LD, CLOCK_HILO);
  executeInstructionOnMC14500(INST_STO, CLOCK_HILO);

  return expect(PIN_WRITE, LOW, "testOEN.WRITE") &&
         expect(PIN_RR, HIGH, "testOEN.RR");
}

bool testSTOH()
{
  bool result = true;

  setIen(HIGH);
  setOen(HIGH);

  setLine(PIN_DATA, HIGH);
  executeInstructionOnMC14500(INST_LD, CLOCK_HILO);
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

  return result;
}

bool testSTOL()
{
  setIen(HIGH);
  setOen(HIGH);

  setLine(PIN_DATA, LOW);
  executeInstructionOnMC14500(INST_LD, CLOCK_HILO);
  executeInstructionOnMC14500(INST_STO, CLOCK_HILO);

  return expect(PIN_DATA, LOW, "testSTOL.DATA") &&
         expect(PIN_WRITE, LOW, "testSTOL.WRITE") &&
         expect(PIN_RR, LOW, "testSTOL.RR");
}

bool testIEN()
{
  setIen(LOW);
  setOen(HIGH);

  setLine(PIN_DATA, HIGH);
  executeInstructionOnMC14500(INST_LD, CLOCK_HILO);

  return expect(PIN_RR, LOW, "testIEN.WRITE");
}
