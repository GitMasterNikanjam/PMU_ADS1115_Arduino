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

    // Last error accured for object.
    String errorMessage;

    struct ParametersStructure
    {
      // Device number. number can be 1, 2, 3 or 4.
      uint8_t DEVICE_NUM;

      /// ADS1115 i2c device address
      uint16_t ADDRESS;                    

      /*
        Data rate in samples per second, based on datasheet numbers.   

      |data_rate---|ADS101x---|ADS 111x------|

      |------------|----------|--------------|

      |0           |128       |8 slowest

      |1           |250       |16  

      |2           |490       |32  

      |3           |920       |64  

      |4           |1600      |128 default

      |5           |2400      |250 

      |6           |3300      |475 

      |7           |3300      |860 fastest
      */
      uint8_t DATA_RATE;                  

      /// Digital input pin on arduino for ALERT/RDY digital output pin. This pin used for conversion ready pin.
      int8_t RDY_PIN; 

      uint8_t RDY_POLARITY;                    
      
      /// Sensitivity of channels. milli volt per user defined unit. [mv/uu]
      float SENSITIVITY[4];

      /// Offsset value for channels. [mvolt]
      float OFFSET[4];

      /// Enable/Disable of channels.
      bool ACTIVE_MODE[4];

      /**
       * Programmable gain amplifier configuration. 

       * PGA value   ,Full scale range Voltage      

       * 0           ±6.144V default    

       * 1           ±4.096V    

       * 2           ±2.048V    

       * 4           ±1.024V 

       * 8           ±0.512V 

       * 16          ±0.256V 

       * Note: other values not acceptable.
       */
      uint8_t PGA;

      // Update frequency. This value ensures that updates occur at a specific frequency. Hint: A value of 0 means it is disabled and update in free frequency.
      float UPDATE_FRQ;
    }parameters;

    struct ValuesStructure
    {
      // Raw 16bit values for channels.
      int16_t raw[4];

      // Converted values by sensitivity and offsets.
      float converted[4];
    }value;

    // ADS1115 object from ADS1X15 library.
    ADS1115 ADS;                          

    /// Default constructor.
    PMU_ADS1115(uint8_t address = 0x48);

    // Destructor.
    ~PMU_ADS1115();

    /**
     * Attach a PMU_ADS1115 device to devices list.
     * Creates an instance of the class for a specific device number and ready pin. If an object for the same channel exists, it is replaced.
     */ 
    bool attach(uint8_t device_number, uint8_t pin_number);

    // Removes an object for a specific device.
    bool detach(void);

    /// Initial object. check parameters.
    bool init(void);

    /// update values or handle ADS1115 conversion.
    void update(void);

  private:

    uint _channel;        /// Channel number of ADS1115 for ready to convertion or read.
    bool _isExist;        /// Flag for ADS1115 conection.
    bool _RDYFlag;        /// Flag for conversion completed.
    float _voltRef;       /// Reference voltage for ADS11115.

    // Static array to store instances per channel
    // Array to hold one object per device (1-4)
    static PMU_ADS1115* _instances[4];

    // Define function pointer type
    typedef void (*FunctionPtr)();

    // FunctionPtr object for RCIN PWM interrupts handler.
    FunctionPtr _funPointer;

    // Flag for store state of channeles that attached(true) or not_attached(false)
    bool _attachedFlag;

    /// Check parameters validation.
    bool _checkParameters(void);

    // ------------------------------------------------
    // Friends functions for interrupts handling.

    friend void _adsReadyDevice_1(void);
    friend void _adsReadyDevice_2(void);
    friend void _adsReadyDevice_3(void);
    friend void _adsReadyDevice_4(void);

};


#endif
