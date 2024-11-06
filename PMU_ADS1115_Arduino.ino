#include "PMU_ADS1115.h"

PMU_ADS1115 PMU(0x49);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  PMU.parameters.ACTIVE_MODE[0] = true;
  PMU.parameters.ACTIVE_MODE[1] = true;
  PMU.parameters.ACTIVE_MODE[2] = true;
  PMU.parameters.DATA_RATE = 4;
  PMU.parameters.OFFSET[0] = 2500;
  PMU.parameters.SENSITIVITY[0] = 50;
  PMU.parameters.PGA = 0;
  PMU.parameters.RDY_PIN = 10;
  PMU.parameters.UPDATE_FRQ = 100;
  
  if(PMU.init() == false)
  {
    Serial.println(PMU.errorMessage);
    while(1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  PMU.update();

  Serial.print(PMU.value.raw[0]); Serial.print(", ");
  Serial.print(PMU.value.raw[1]); Serial.print(", ");
  Serial.println(PMU.value.raw[2]);

  delay(10);

}
