#ifndef PMU_ADS1115_H
#define PMU_ADS1115_H

#include <Arduino.h>
#include <Wire.h>              // Standard Arduino I2C Library exist at arduino IDE . https://www.arduino.cc/reference/en/language/functions/communication/wire/
#include <ADS1X15.h> 

#define ADS_RAW_RANGE             32767.0     // 16 bit resolution. int16 value from get ADS channels
#define ADS_MAX_VOLTAGE           6.144       // reference ADC voltage

class PMU_ADS1115
{

  public:

    struct ParametersStruct
    {
      uint16_t ADDRESS;          // ADS1115 i2c device address -> Address pin connected to VCC.
      uint8_t DATA_RATE;        // 5: 250 samples per second
      int8_t RDY_PIN;
    }parameters;

    ADS1115 ADS;

    PMU_ADS1115(uint8_t address = 0x48);

    ~PMU_ADS1115();

    bool init(void);

  private:

    bool _ads_exist;
    bool _ads_flag;

    // Static array to store instances per channel
    // Array to hold one object per channel (1-8)
    static PMU_ADS1115* _instances;

    // Define function pointer type
    typedef void (*FunctionPtr)();

    // FunctionPtr object for RCIN PWM interrupts handler.
    FunctionPtr _funPointer;

    // Flag for store state of channeles that attached(true) or not_attached(false)
    bool _attachedFlag;

    friend void _ADS_adsReady(void);
};


#endif