#ifndef PMU_ADS1115_H
#define PMU_ADS1115_H

// ############################################################################
// Include libraries:

#include <Arduino.h>          // Main include file for the Arduino SDK
#include <Wire.h>             // Standard Arduino I2C Library exist at arduino IDE . https://www.arduino.cc/reference/en/language/functions/communication/wire/
#include <ADS1X15.h>          // https://github.com/RobTillaart/ADS1X15

// #################################################################################
// Define macros:



// ###############################################################################
// PMU_ADS1115 class:

class PMU_ADS1115
{

  public:

    struct ParametersStructure
    {
      uint16_t ADDRESS;                   // ADS1115 i2c device address -> Address pin connected to VCC.
      uint8_t DATA_RATE;                  // 5: 250 samples per second
      int8_t RDY_PIN;
    }parameters;

    float value[4];

    ADS1115 ADS;                          // ADS1115 object from ADS1X15 library.

    PMU_ADS1115(uint8_t address = 0x48);

    ~PMU_ADS1115();

    bool init(void);

    void update(void);

  private:

    uint16_t ADS_val[4];

    uint ADS_channel;

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

    bool _checkParameters(void);

    friend void _ADS_adsReady(void);
};


#endif