// ##################################################################
// Include libraries:

#include "PMU_ADS1115.h"

// ###################################################################
// Define macros:

#define ADS1115_RAW_RANGE             32767.0     // 16 bit resolution. int16 value from get ADS channels
#define ADS1115_DEFAULT_ADD           0x48        // Default I2C address of ADS1115 ADC.

// ##################################################################
// Initial static members: 

PMU_ADS1115* PMU_ADS1115::_instances[4] = {nullptr, nullptr, nullptr, nullptr};

// ##################################################################
// Friends functions:

void _adsReadyDevice_1(void)
{
  PMU_ADS1115::_instances[0]->_RDYFlag = true;
}

void _adsReadyDevice_2(void)
{
  PMU_ADS1115::_instances[1]->_RDYFlag = true;
}

void _adsReadyDevice_3(void)
{
  PMU_ADS1115::_instances[2]->_RDYFlag = true;
}

void _adsReadyDevice_4(void)
{
  PMU_ADS1115::_instances[3]->_RDYFlag = true;
}

// ##################################################################
// PMU_ADS1115 class:

PMU_ADS1115::PMU_ADS1115()
{
  parameters.ADDRESS = ADS1115_DEFAULT_ADD;
  parameters.DATA_RATE = 4;
  parameters.RDY_PIN = -1;

  _isExist = false;

}

PMU_ADS1115::~PMU_ADS1115()
{
  detach();
  delete _ADS;
}

bool PMU_ADS1115::attachRdyPin(uint8_t RDY_pinNumber, uint8_t deviceNumber)
{
  if( (deviceNumber > 4) || (deviceNumber == 0) )
  {
    errorMessage = "Error PMU_ADS1115: device number is not correct.";
    return false;
  }

  parameters.RDY_PIN = RDY_pinNumber;
  _attachedFlag = true;
  
  return true;
}

void PMU_ADS1115::detachRdyPin(void) 
{
  // Detach the interrupt and delete the object
  detachInterrupt(digitalPinToInterrupt(parameters.RDY_PIN));
  _instances[parameters.DEVICE_NUM - 1] = nullptr;
  _attachedFlag = false;
}

bool PMU_ADS1115::init(void)
{
  if(!_checkParameters())
  {
    return false;
  }

  // Create a new object for this channel
  _instances[deviceNumber - 1] = this;

  parameters.DEVICE_NUM = deviceNumber;

  _ADS = new ADS1115(parameters.ADDRESS);

  if(_ADS->isConnected())
  {
    _isExist = true;
    _ADS->begin();
    _ADS->setGain(parameters.PGA);        // 6.144 volt --> max voltage read
    _ADS->setDataRate(parameters.DATA_RATE); 
      
    // SET ALERT RDY PIN
    _ADS->setComparatorThresholdHigh(0x8000);
    _ADS->setComparatorThresholdLow(0x0000);
    _ADS->setComparatorQueConvert(0);
    
    if( (_attachedFlag == true) && (parameters.RDY_PIN >= 0) )
    {
      pinMode(parameters.RDY_PIN,INPUT_PULLUP);
      
      _funPointer = nullptr;
      
      switch(parameters.DEVICE_NUM)
      {
        case 1:
          _funPointer = _adsReadyDevice_1;
        break;
        case 2:
          _funPointer = _adsReadyDevice_2;
        break;
        case 3:
          _funPointer = _adsReadyDevice_3;
        break;
        case 4:
          _funPointer = _adsReadyDevice_4;
        break;		
      }

      if(parameters.RDY_POLARITY == 0)
      {
        _ADS->setComparatorPolarity(LOW);
        attachInterrupt(digitalPinToInterrupt(parameters.RDY_PIN), _funPointer, FALLING);
      }
      else if(parameters.RDY_POLARITY == 1)
      {
        _ADS->setComparatorPolarity(HIGH);
        attachInterrupt(digitalPinToInterrupt(parameters.RDY_PIN), _funPointer, RISING);
      }
      else
      {
        return false;
      }
    
    }
    else
    {
      _attachedFlag = false;
      return false;
    }

    _ADS->setMode(0);               // Continuse mode
    _ADS->readADC(0);     // trigger first read
  }
  else
  {
    _isExist = 0;
    errorMessage = "ADS Sensor Not Connected!";
    return false;
  }
  return true;
}

void PMU_ADS1115::update(void)
{
  if (_RDYFlag)
  {
    // save the value
    value.raw[_channel] = _ADS->getValue();

    switch(_channel)
    {
    case 0:
      value.converted[0] =(1000.0 * value.raw[0]/ADS1115_RAW_RANGE * _voltRef - parameters.OFFSET[0]) / parameters.SENSITIVITY[0];
    break;
    case 1:
      value.converted[1] =(1000.0 * value.raw[1]/ADS1115_RAW_RANGE * _voltRef - parameters.OFFSET[1]) / parameters.SENSITIVITY[1];
    break;
    case 2:
      value.converted[2] =(1000.0 * value.raw[2]/ADS1115_RAW_RANGE * _voltRef - parameters.OFFSET[2]) / parameters.SENSITIVITY[2];
    break;
    case 3:
      value.converted[3] =(1000.0 * value.raw[3]/ADS1115_RAW_RANGE * _voltRef - parameters.OFFSET[3]) / parameters.SENSITIVITY[3];
    break;
    }

    // request next channel
    do
    {
      _channel++;
      if (_channel > 3)
      {
        _channel = 0;
      } 
    }while(parameters.ACTIVE_MODE[_channel] == false);
    
    _ADS->readADC(_channel);
    _RDYFlag = false;
  }
}


bool PMU_ADS1115::_checkParameters(void)
{
  bool state = (_instances[parameters.DEVICE_NUM - 1] == nullptr) &&
               (parameters.DEVICE_NUM <= 4) && (parameters.DEVICE_NUM >= 1) &&
               (parameters.ADDRESS >= 0x48) && (parameters.ADDRESS <= 0x4B) &&
               (parameters.DATA_RATE)

  return true;
}
