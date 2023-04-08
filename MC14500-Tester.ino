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

#include "src/test_harness.h"
#include "src/test_harness_14500.h"
#include "src/tests_MC14500.h"

void setup()
{
  releaseMC14500();
  setupTestHarness();

  Serial.begin(9600);
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

  Serial.println(F("------------- START -------------"));

  powerUpMC14500();

  Serial.println(F("Power up                       OK"));

  result = testIEN() &&
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

  digitalWrite(PIN_LED_GREEN, result);
  digitalWrite(PIN_LED_RED, !result);

  releaseMC14500();

  Serial.println(F("Release DUT                    OK"));
  Serial.println(F("-------------  END  -------------"));
  Serial.println("");

  // Hang here in case of failure so it's not missed
  // and we can leave the system unattended in ageing
  // mode while still catching random failures.
  if (!result)
  {
    while (true)
      ;
  }

  delay(2000);
}
