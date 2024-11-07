// ############################################################################
// Include libraries:

#include <PMU_ADS1115.h>

// ############################################################################
// Global objects and variables:

PMU_ADS1115 pmu;

// ###############################################################################
// Setup:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pmu.parameters.ACTIVE_MODE[0] = true;
  pmu.parameters.ACTIVE_MODE[1] = true;
  pmu.parameters.ACTIVE_MODE[2] = true;
  pmu.parameters.DATA_RATE = 4;
  pmu.parameters.OFFSET[0] = 2500;
  pmu.parameters.SENSITIVITY[0] = 50;
  pmu.parameters.PGA = 0;
  pmu.parameters.RDY_PIN = 10;
  pmu.parameters.UPDATE_FRQ = 100;
  pmu.parameters.ADDRESS = 0x49;
  pmu.parameters.DEVICE_NUM = 1;
  
  if(pmu.init() == false)
  {
    Serial.println(pmu.errorMessage);
    while(1);
  }
}

// ###############################################################################
// Loop

void loop() {
  // put your main code here, to run repeatedly:
  pmu.update();

  Serial.print(pmu.value.raw[0]); Serial.print(", ");
  Serial.print(pmu.value.raw[1]); Serial.print(", ");
  Serial.println(pmu.value.raw[2]);

  delay(10);

}
