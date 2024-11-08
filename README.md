# PMU_ADS1115 Library for Arduino

- This library can be used for a power module unit (PMU) as part of projects.     
- This library uses the <ADS1X15.h> library by Rob Tillaart for communication with the ADS1115 ADC.    
- Reference repository for the <ADS1X15.h> library: https://github.com/RobTillaart/ADS1X15     
- With this library, it is only possible and allowed to create up to 4 objects from the PMU_ADS1115 class.  
- The RDY pin of the ADS1115 ADC must be used and connected to the MCU; otherwise, the PMU_ADS1115 cannot initialize successfully.       

ℹ️ **Notice:**    
  Each PMU_ADS1115 object uses an independent ADS1115 ADC IC. Therefore, each object should use a different I2C slave device address.   

<div style="border: 1px solid #ccc; font-size: 16px; color: #000000; padding: 10px; background-color: #ffff00;">
<strong>Notice:</strong>  
The I2C device address for each object can be 0x48, 0x49, 0x4A, or 0x4B.    
</div>

## Public Member Functions

```c++

/**
     * Initial object. check parameters.
     * @return true if successed.
     */
    bool init(void);

    /**
      @brief update values or handle ADS1115 conversion.  
      @note In each update execution, a maximum of one channel of the ADS1115 ADC updates its value.   
    */ 
    void update(void);

```

## Public Member Variables

```c++

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

```
-------------------------------------------------------
# RobTillaart/ADS1X15 library user Guide 

Here’s a basic guide to help you get started with the RobTillaart/ADS1X15 library, which is often used to interface with the ADS1115 and ADS1015 ADCs:

### Installation

To install the ADS1X15 library:    
Open the Arduino IDE.    
Go to Sketch > Include Library > Manage Libraries.     
Search for ADS1X15 by Rob Tillaart and install the latest version.    
Alternatively, you can download it from GitHub and add it to the Arduino libraries folder.    

### Library Overview

The ADS1X15 library supports two analog-to-digital converters (ADC) from Texas Instruments:   
ADS1115: 16-bit ADC with a programmable gain amplifier (PGA).   
ADS1015: 12-bit ADC with similar features but faster conversion.   
These ADCs allow for high-resolution analog input measurements, commonly used in applications requiring precise voltage readings.    

This library should work for the devices mentioned below, although not all sensors support all functionality.

|Device    |Channels  |Resolution  |Max sps   |Comparator  |ProgGainAMP|
|----------|----------|------------|----------|------------|-----------| 
|ADS1013   |1         |12          |3300      |N           |N          |
|ADS1014   |1         |12          |3300      |Y           |Y          |
|ADS1015   |4         |12          |3300      |Y           |Y          | 
|ADS1113   |1         |16          |860       |N           |N          | 
|ADS1114   |1         |16          |860       |Y           |Y          | 
|ADS1115   |4         |16          |860       |Y           |Y          | 

As the 1015 and the 1115 are both 4 channels these are the most interesting from functionality point of view as these can also do differential measurement.   

### Interface
The address of the ADS1113/4/5 is determined by to which pin the ADDR is connected to:   

ADDR pin connected to Address Notes  
GND 0x48 (default)   
VDD 0x49     
SDA 0x4A    
SCL 0x4B  

### Initializing

To initialize the library you must call a constructor as described below.    

```ADS1x15()``` constructor, should not be used.    
```ADS1013(address, TwoWire *wire = &Wire)``` Constructor with device address, and optional the Wire interface as parameter.   
```ADS1014(address, TwoWire *wire = &Wire)``` Constructor with device address, and optional the Wire interface as parameter.    
```ADS1015(address, TwoWire *wire = &Wire)``` Constructor with device address, and optional the Wire interface as parameter.   
```ADS1113(address, TwoWire *wire = &Wire)``` Constructor with device address, and optional the Wire interface as parameter.    
```ADS1114(address, TwoWire *wire = &Wire)``` Constructor with device address, and optional the Wire interface as parameter.   
```ADS1115(address, TwoWire *wire = &Wire)``` Constructor with device address, and optional the Wire interface as parameter.    

The function void ```setWireClock(uint32_t speed)``` is used to set the clock speed of the used I2C interface.    

The function ```uint32_t getWireClock()``` is a prototype. It returns the value set by ```setWireClock()```. This is not necessary the actual value. When no value is set ```getWireClock()``` returns 0. Need to implement a read / calculate from low level I2C code (e.g. TWBR on AVR), better the Arduino Wire lib should support this call (ESP32 does).

After construction the ```ADS.begin()``` need to be called. This will return false if an invalid address is used. The function bool ```isConnected()``` can be used to verify the reading of the ADS. The function void ```reset()``` is sets the parameters to their initial value as in the constructor.   

For example:

```C++
#include "ADS1X15.h"

// initialize ADS1115 on I2C bus 1 with default address 0x48
ADS1115 ADS(0x48);

void begin() {
  if (!ADS.isConnected()) {
    // error ADS1115 not connected
  }
}
```

### Programmable Gain
void ```setGain(uint8_t gain)``` set the gain value, indicating the maxVoltage that can be measured Adjusting the gain allows one to make more precise measurements. Note: the gain is not set in the device until an explicit read/request of the ADC (any read call will do). See table below.   
uint8_t getGain() returns the gain value (index).    

|PGA value   |Full scale range Voltage     | 
|------------|-----------------------------|
|0           |±6.144V default 
|1           |±4.096V 
|2           |±2.048V 
|4           |±1.024V 
|8           |±0.512V 
|16          |±0.256V 

float ```getMaxVoltage()``` returns the max voltage with the current gain.    
float ```toVoltage(int16_t raw = 1)``` converts a raw measurement to a voltage. Can be used for normal and differential measurements. The default value of 1 returns the conversion factor for any raw number.
The voltage factor can also be used to set HIGH and LOW threshold registers with a voltage in the comparator mode. Check the examples.

```
 float f = ADS.toVoltage(); 
 ADS.setComparatorThresholdLow( 3.0 / f );
 ADS.setComparatorThresholdLow( 4.3 / f );
```

### Operational mode

The ADS sensor can operate in single shot or continuous mode. Depending on how often one needs a conversion one can tune the mode.  

```void setMode(uint8_t mode)``` 0 = CONTINUOUS, 1 = SINGLE (default) Note: the mode is not set in the device until an explicit read/request of the ADC (any read call will do).  

```uint8_t getMode()``` returns current mode 0 or 1, or ADS1X15_INVALID_MODE = 0xFE.  

### Data rate

```void setDataRate(uint8_t dataRate)``` Data rate depends on type of device. For all devices the index 0..7 can be used, see table below. Values above 7 ==> will be set to the default 4. Note: the data rate is not set in the device until an explicit read/request of the ADC (any read call will do).
uint8_t ```getDataRate()``` returns the current data rate (index).   
The library has no means to convert this index to the actual numbers as that would take 32 bytes.   

Data rate in samples per second, based on datasheet numbers.   

|data_rate   |ADS101x   |ADS 111x      |
|------------|----------|--------------|
|0           |128       |8 slowest
|1           |250       |16  
|2           |490       |32  
|3           |920       |64  
|4           |1600      |128 default
|5           |2400      |250 
|6           |3300      |475 
|7           |3300      |860 fastest  

### ReadADC Single mode

Reading the ADC is very straightforward, the ```readADC()``` function handles all in one call. Under the hood it uses the asynchronous calls.   

```int16_t readADC(uint8_t pin = 0)``` normal ADC functionality, pin = 0..3. If the pin number is out of range, this function will return 0.
Default pin = 0 as this is convenient for 1 channel devices.  

```
// read ADC in pin 2
ADS.readADC(2);

// read ADC in pin 0 - two ways
ADS.readADC();
ADS.readADC(0);
```

To read the ADC in an asynchronous way (e.g. to minimize blocking) one has to use three calls:    

```void requestADC(uint8_t pin)``` Start the conversion. pin = 0..3.   
```bool isBusy()``` Is the conversion not ready?   
```bool isReady()``` Is the conversion ready? (= wrapper around isBusy() )   
```int16_t getValue()``` Read the result of the conversion.   

in terms of code:  

```C++
  void setup()
  {
    // other setup things here
    ADS.setMode(1);               // SINGLE SHOT MODE
    ADS.requestADC(pin);
  }

  void loop()
  {
    if (ADS.isReady())
    {
      value = ADS.getValue();
      ADS.requestADC(pin);       // request new conversion
    }
    // do other things here
  }
```

### ReadADC Differential

For reading the ADC in a differential way there are 4 calls possible.   

```int16_t readADC_Differential_0_1()``` returns the difference between 2 ADC pins.   
```int16_t readADC_Differential_0_3()``` ADS1x15 only   
```int16_t readADC_Differential_1_3()``` ADS1x15 only   
```int16_t readADC_Differential_2_3()``` ADS1x15 only    
```int16_t readADC_Differential_0_2()``` ADS1x15 only - in software (no async equivalent)    
```int16_t readADC_Differential_1_2()``` ADS1x15 only - in software (no async equivalent)    

```
// read differential ADC between pin 0 and 1
ADS.readADC_Differential_0_1(0);
```

The differential reading of the ADC can also be done with asynchronous calls.  

```int16_t requestADC_Differential_0_1()``` starts conversion for differential reading   
```int16_t requestADC_Differential_0_3()``` ADS1x15 only   
```int16_t requestADC_Differential_1_3()``` ADS1x15 only    
```int16_t requestADC_Differential_2_3()``` ADS1x15 only    

After one of these calls one need to call   
```int16_t getValue()``` Read the result of the last conversion.     

```bool isBusy()``` Is the conversion ready?
int16_t getValue() Read the result of the last conversion.

###  ReadADC continuous mode

To use the continuous mode one need three calls   

```void setMode(0)``` 0 = CONTINUOUS, 1 = SINGLE (default). 

- **Note:** the mode is not set in the device until an explicit read/request of the ADC (any read call will do).   

```int16_t readADC(uint8_t pin)``` or ```void requestADC(uint8_t pin)``` to get the continuous mode started.   
```int16_t getValue()``` to return the last value read by the device. Note this can be a different pin, so be warned. Calling this over and over again can give the same value multiple times.    
By using ```bool isBusy()``` or ```bool isReady()`` one can wait until new data is available. Note this only works in the SINGLE_SHOT modus.   

In continuous mode, you can't use ```isBusy()``` or ```isReady()``` functions to wait until new data available.   
Instead you can configure the threshold registers to allow the ALERT/RDY pin to trigger an interrupt signal when conversion data ready.

### Threshold registers ==> mode RDY pin

If the thresholdHigh is set to 0x0100 and the thresholdLow to 0x0000 the ALERT/RDY pin is triggered when a conversion is ready.  

```void setComparatorThresholdLow(int16_t lo)``` writes value to device directly.    
```void setComparatorThresholdHigh(int16_t hi)``` writes value to device directly.   
```int16_t getComparatorThresholdLow()``` reads value from device.    
```int16_t getComparatorThresholdHigh()``` reads value from device.   

### Comparator

- NOTE: all comparator settings are copied to the device only after an explicit ```readADC()``` or ```requestADC()```   

#### Comparator Mode  

When configured as a TRADITIONAL comparator, the ALERT/RDY pin asserts (active low by default) when conversion data exceed the limit set in the high threshold register. The comparator then de-asserts when the input signal falls below the low threshold register value.   

```void setComparatorMode(uint8_t mode)``` value 0 = TRADITIONAL 1 = WINDOW,   
```uint8_t getComparatorMode()```   
If the comparator LATCH is set, the ALERT/RDY pin asserts and it will be reset after reading the sensor (conversion register) again. An SMB alert command (00011001) on the I2C bus will also reset the alert state. Not implemented in the library (yet)   

In WINDOW comparator mode, the ALERT/RDY pin asserts if conversion data exceeds the high threshold register or falls below the low threshold register. In this mode the alert is held if the LATCH is set. This is similar as above.   

#### Polarity

Default state of the ALERT/RDY pin is LOW, can be to set HIGH.

```void setComparatorPolarity(uint8_t pol)``` Flag is only explicitly set after a ```readADC()``` or a ```requestADC()```   
```uint8_t getComparatorPolarity()``` returns value set.   

Latch   
Holds the ALERT/RDY to HIGH (or LOW depending on polarity) after triggered even if actual value has been 'restored to normal' value.

```void setComparatorLatch(uint8_t latch)``` 0 = NO LATCH, not 0 = LATCH   
```uint8_t getComparatorLatch()``` returns value set.   

QueConvert   
Set the number of conversions before trigger activates. The ```void setComparatorQueConvert(uint8_t mode)``` is used to set the number of conversions that exceed the threshold before the ALERT/RDY pin is set HIGH. A value of 3 (or above) effectively disables the comparator. See table below.

```void setComparatorQueConvert(uint8_t mode)``` See table below.   
```uint8_t getComparatorQueConvert()``` returns value set.   

|value |meaning Notes                       |
|------|------------------------------------|
|0     |trigger alert after 1 conversion  
|1     |trigger alert after 2 conversions 
|2     |trigger alert after 4 conversions 
|3     |Disable comparator  default

#### Threshold registers comparator mode  

Depending on the comparator mode TRADITIONAL or WINDOW the thresholds registers mean something different see - Comparator Mode above or datasheet.   

```void setComparatorThresholdLow(int16_t lo)``` set the low threshold; take care the hi >= lo.    
```void setComparatorThresholdHigh(int16_t hi)``` set the high threshold; take care the hi >= lo.   
```int16_t getComparatorThresholdLow()``` reads value from device.    
```int16_t getComparatorThresholdHigh()``` reads value from device.   

--------------------------------------------------------------------


