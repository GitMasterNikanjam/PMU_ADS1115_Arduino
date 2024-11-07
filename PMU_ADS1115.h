#ifndef PMU_ADS1115_H
#define PMU_ADS1115_H

// ############################################################################
// Include libraries:

#include <Arduino.h>          // Main include file for the Arduino SDK
#include <Wire.h>             // Standard Arduino I2C Library exist at arduino IDE . https://www.arduino.cc/reference/en/language/functions/communication/wire/
#include <ADS1X15.h>          // https://github.com/RobTillaart/ADS1X15

// #################################################################################
// Define Global Macros:



// ###############################################################################
// PMU_ADS1115 class:

/**
  @class PMU_ADS1115
  @brief Class for power management unit with ADS1115 ADC.
*/
class PMU_ADS1115
{
  public:

    /// @brief Last error accured for object.
    String errorMessage;

    /**
      @struct ParametersStructure
      @brief Parameters structure.
    */
    struct ParametersStructure
    {
      /**
        @brief Device number. Number can be just 1, 2, 3 or 4. Other values is not correct.
      */ 
      uint8_t DEVICE_NUM;

      /**
        @brief ADS1115 I2C device address. The I2C device address for each object can be 0x48, 0x49, 0x4A, or 0x4B.  
        
          ADDR pin connected to Address Notes:  

          GND 0x48 (default)    

          VDD 0x49     

          SDA 0x4A    

          SCL 0x4B 
      */ 
      uint16_t ADDRESS;                    

      /**
        @brief Data rate in samples per second, based on datasheet numbers.   

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

      /// @brief Digital input pin on arduino for ALERT/RDY digital output pin. This pin used for conversion ready pin.
      int8_t RDY_PIN; 

      /// @brief RDY pin signal polarity. 0: Active LOW, 1: Active High.
      uint8_t RDY_POLARITY;                    
      
      /// @brief Sensitivity of channels. milli volt per user defined unit. [mv/uu]
      float SENSITIVITY[4];

      /// @brief Offsset value for channels. [mvolt]
      float OFFSET[4];

      /// @brief Enable/Disable channels of the ADS1115 ADC. If a channel is disabled, then its value is not updated.
      bool ACTIVE_MODE[4];

      /**
       * @brief Programmable gain amplifier configuration. 

       * PGA value   ,Full scale range Voltage      

       * 0           ±6.144V default    

       * 1           ±4.096V    

       * 2           ±2.048V    

       * 4           ±1.024V 

       * 8           ±0.512V 

       * 16          ±0.256V 

       * @note  other values not acceptable.
       */
      uint8_t PGA;

      /// @brief Update frequency. This value ensures that updates occur at a specific frequency. Hint: A value of 0 means it is disabled and update in free frequency.
      float UPDATE_FRQ;
    }parameters;

    /**
      @struct ValuesStructure
      @brief Value structure.
    */
    struct ValuesStructure
    {
      /// @brief Raw 16bit values for channels.
      int16_t raw[4];

      /// @brief Converted values by sensitivity and offsets parameters.
      float converted[4];
    }value;                         

    /// @brief Default constructor. Init some variables.
    PMU_ADS1115();

    /// @brief Destructor.
    ~PMU_ADS1115();

    /**
     * Attach a PMU_ADS1115 device to devices list.
     * Creates an instance of the class for a specific device number and ready pin. If an object for the same channel exists, it is replaced.
     */ 
    bool attach(uint8_t RDY_pinNumber, uint8_t deviceNumber = 1);

    /// @brief Removes an object for a specific device.
    void detach(void);

    /// Initial object. check parameters.
    bool init(void);

    /// update values or handle ADS1115 conversion.
    void update(void);

  private:

    uint _channel;        /// Channel number of ADS1115 for ready to convertion or read.
    bool _isExist;        /// Flag for ADS1115 conection.
    bool _RDYFlag;        /// Flag for conversion completed.
    float _voltRef;       /// Reference voltage for ADS11115.

    /// @brief ADS1115 pointer object from ADS1X15 library.
    ADS1115 *_ADS; 

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
