#include "PMU_ADS1115.h"

PMU_ADS1115 PMU(0x49);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  PMU.init();
}

void loop() {
  // put your main code here, to run repeatedly:

}
