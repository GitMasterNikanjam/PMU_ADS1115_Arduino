// ############################################################################
// Include libraries:

#include "PMU_ADS1115.h"

// ############################################################################
// Global objects and variables:

PMU_ADS1115 pmu;

// ###############################################################################
// Setup:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pmu.parameters.ADDRESS = 0x49;
  pmu.parameters.DEVICE_NUM = 1;
  pmu.parameters.RDY_PIN = 10;
  pmu.init();
}

// ###############################################################################
// Loop

void loop() {
  // put your main code here, to run repeatedly:

}
