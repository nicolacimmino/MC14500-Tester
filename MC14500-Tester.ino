// Tester for MC14500 ICUs.
//  Copyright (C) 2023 Nicola Cimmino
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see http://www.gnu.org/licenses/.
//
//

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

#define PIN_BUTTON A6
#define PIN_LED_RED A5
#define PIN_LED_GREEN A4

#define DUT_PINS_COUNT 16

#define INST_NOPO 0x0
#define INST_LD 0x1
#define INST_LDC 0x2
#define INST_OR 0x3
#define INST_ORC 0x4
#define INST_AND 0x5
#define INST_ANDC 0x6
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

#define RX_BUFFER_SIZE 256
byte rxBuffer[RX_BUFFER_SIZE];

void releaseDut()
{
  for (uint8_t ix = 0; ix < DUT_PINS_COUNT; ix++)
  {
    pinMode(dut_pins[ix], INPUT);
  }
}

void powerUpDut()
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

void executeInstructionOnDut(byte instruction, byte clockPhases)
{
  pinMode(PIN_I0, OUTPUT);
  pinMode(PIN_I1, OUTPUT);
  pinMode(PIN_I2, OUTPUT);
  pinMode(PIN_I3, OUTPUT);

  digitalWrite(PIN_I0, instruction & 0x1);
  digitalWrite(PIN_I1, (instruction >> 1) & 0x1);
  digitalWrite(PIN_I2, (instruction >> 2) & 0x1);
  digitalWrite(PIN_I3, (instruction >> 3) & 0x1);

  clockDut(clockPhases);
}

void clockDut(byte phases)
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

void setup()
{
  releaseDut();

  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_RED, LOW);

  Serial.begin(9600);
}

bool setIen(byte status)
{
  setLine(PIN_DATA, status);
  executeInstructionOnDut(INST_IEN, CLOCK_HILO);
}

bool setOen(byte status)
{
  setLine(PIN_DATA, status);
  executeInstructionOnDut(INST_OEN, CLOCK_HILO);
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

bool testLD()
{
  setIen(HIGH);
  setOen(HIGH);

  setLine(PIN_DATA, HIGH);
  executeInstructionOnDut(INST_LD, CLOCK_HILO);

  return expect(PIN_RR, HIGH, "testLD.RR");
}

bool testLDC()
{
  setIen(HIGH);
  setOen(HIGH);

  setLine(PIN_DATA, HIGH);
  executeInstructionOnDut(INST_LDC, CLOCK_HILO);

  return expect(PIN_RR, LOW, "testLDC.RR");
}

bool testOEN()
{
  setIen(HIGH);
  setOen(LOW);

  setLine(PIN_DATA, HIGH);
  executeInstructionOnDut(INST_LD, CLOCK_HILO);
  executeInstructionOnDut(INST_STO, CLOCK_HILO);

  return expect(PIN_WRITE, LOW, "testOEN.WRITE") &&
         expect(PIN_RR, HIGH, "testOEN.RR");
}

bool testSTOH()
{
  bool result = true;

  setIen(HIGH);
  setOen(HIGH);

  setLine(PIN_DATA, HIGH);
  executeInstructionOnDut(INST_LD, CLOCK_HILO);
  executeInstructionOnDut(INST_STO, CLOCK_HI);

  result = result &&
           expect(PIN_DATA, HIGH, "testSTOH.DATA") &&
           expect(PIN_WRITE, HIGH, "testSTOH.WRITEH") &&
           expect(PIN_RR, HIGH, "testSTOH.RR");

  clockDut(CLOCK_LO);

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
  executeInstructionOnDut(INST_LD, CLOCK_HILO);
  executeInstructionOnDut(INST_STO, CLOCK_HILO);

  return expect(PIN_DATA, LOW, "testSTOL.DATA") &&
         expect(PIN_WRITE, LOW, "testSTOL.WRITE") &&
         expect(PIN_RR, LOW, "testSTOL.RR");
}

bool testIEN()
{
  setIen(LOW);
  setOen(HIGH);

  setLine(PIN_DATA, HIGH);
  executeInstructionOnDut(INST_LD, CLOCK_HILO);

  return expect(PIN_RR, LOW, "testIEN.WRITE");
}

bool isButtonPressed()
{
  return analogRead(PIN_BUTTON) < 5;
}

void loop()
{
  bool result = true;

  digitalWrite(PIN_LED_GREEN, LOW);

  while (!isButtonPressed())
  {
    digitalWrite(PIN_LED_RED, (millis() % 1000 < 200) ? HIGH : LOW);
  }

  digitalWrite(PIN_LED_RED, LOW);

  powerUpDut();

  result = testIEN() &&
           testOEN() &&
           testLD() &&
           testLDC() &&
           testSTOH() &&
           testSTOL();

  digitalWrite(PIN_LED_GREEN, result);
  digitalWrite(PIN_LED_RED, !result);

  releaseDut();

  delay(2000);
}
