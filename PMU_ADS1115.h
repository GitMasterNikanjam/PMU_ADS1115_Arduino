#ifndef PMU_ADS1117_H
#define PMU_ADS1117_H

#include <Arduino.h>
#include <Wire.h>              // Standard Arduino I2C Library exist at arduino IDE . https://www.arduino.cc/reference/en/language/functions/communication/wire/
#include <ADS1X15.h> 

#define ADS_INB_DEVICE_ADDRESS    0x49        // ADS1115 i2c device address -> Address pin connected to VCC.
#define ADS_RAW_RANGE             32767.0     // 16 bit resolution. int16 value from get ADS channels
#define ADS_MAX_VOLTAGE           6.144       // reference ADC voltage
#define ADS_DATA_RATE             4           // 5: 250 samples per second

class PMU_ADS1117
{

  public:

    struct ParametersStruct
    {
      uint16_t ADS_ADDRESS;          // ADS1115 i2c device address -> Address pin connected to VCC.
    }parameters;

    ADS1115 ADS(0);

    bool init(void);

  private:

};


#endif