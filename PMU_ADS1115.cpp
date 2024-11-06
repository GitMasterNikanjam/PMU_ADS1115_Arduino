// ##################################################################
// Include libraries:

#include "PMU_ADS1115.h"

// ###################################################################
// Define macros:

#define ADS1115_RAW_RANGE             32767.0     // 16 bit resolution. int16 value from get ADS channels

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

PMU_ADS1115::PMU_ADS1115(uint8_t address)
{
  parameters.ADDRESS = address;
  ADS = ADS1115(parameters.ADDRESS);

  parameters.DATA_RATE = 4;
  parameters.RDY_PIN = -1;

  parameters.DEVICE_NUM = 0;
  parameters.PGA = 0;
  parameters.RDY_POLARITY = 0;
  parameters.UPDATE_FRQ = 0;

  for(int i = 0; i <= 3; ++i)
  {
    value.raw[i] = 0;
    value.converted[i] = 0;
    parameters.ACTIVE_MODE[i] = false;
    parameters.OFFSET[i] = 0;
    parameters.SENSITIVITY[i] = 1;
  }

  _channel = 0;
  _isExist = false;
  _RDYFlag = false;
  _voltRef = 1;
  _attachedFlag = false;

}

PMU_ADS1115::~PMU_ADS1115()
{
  detach();
}

bool PMU_ADS1115::attach(uint8_t deviceNumber, int8_t RDY_pinNumber)
{
  if( (deviceNumber > 4) || (deviceNumber == 0) )
  {
    errorMessage = "Error PMU_ADS1115: Device number is not correct.";
    return false;
  }

  if(RDY_pinNumber < 0)
  {
    errorMessage = "Error PMU_ADS1115: RDY pin number is not valid.";
    return false;
  }

  if (_instances[deviceNumber - 1] != nullptr) 
  {
      // Detach any existing object for this channel
      _instances[deviceNumber - 1]->detach();
      delete _instances[deviceNumber - 1];
  }

  // Create a new object for this channel
  _instances[deviceNumber - 1] = this;

  parameters.DEVICE_NUM = deviceNumber;
  parameters.RDY_PIN = RDY_pinNumber;
  _attachedFlag = true;
  
  return true;
}

void PMU_ADS1115::detach(void) 
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

  if(ADS.isConnected())
  {
    _isExist = true;
    ADS.begin();
    ADS.setGain(parameters.PGA);        // 6.144 volt --> max voltage read
    ADS.setDataRate(parameters.DATA_RATE); 
    ADS.setComparatorPolarity(parameters.RDY_POLARITY);
      
    // SET ALERT RDY PIN
    ADS.setComparatorThresholdHigh(0x8000);
    ADS.setComparatorThresholdLow(0x0000);
    ADS.setComparatorQueConvert(0);
    
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
        ADS.setComparatorPolarity(LOW);
        attachInterrupt(digitalPinToInterrupt(parameters.RDY_PIN), _funPointer, FALLING);
      }
      else if(parameters.RDY_POLARITY == 1)
      {
        ADS.setComparatorPolarity(HIGH);
        attachInterrupt(digitalPinToInterrupt(parameters.RDY_PIN), _funPointer, RISING);
      }
      else
      {
        errorMessage = "Error PMU_ADS1115: Init not successed. ADS1115 device not connected.";
        return false;
      }
    
    }
    else
    {
      errorMessage = "Error PMU_ADS1115: Init not successed.";
      return false;
    }

    ADS.setMode(0);            // Continuse mode

    for(int i = 0 ; i <= 3; ++i)
    {
      if(parameters.ACTIVE_MODE[i] == true)
      {
        _channel = i;
        ADS.readADC(_channel);            // trigger first read
        break;
      }
    }

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
    value.raw[_channel] = ADS.getValue();

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
    
    ADS.readADC(_channel);
    _RDYFlag = false;
  }
}


bool PMU_ADS1115::_checkParameters(void)
{
  bool state = (parameters.DATA_RATE <= 7) &&
               (parameters.DEVICE_NUM >= 1) && (parameters.DEVICE_NUM <= 4) &&
               (parameters.PGA <= 16) &&
               (parameters.RDY_PIN >= 0) &&
               (parameters.SENSITIVITY > 0) &&
               (parameters.RDY_POLARITY <= 1) &&
               (parameters.UPDATE_FRQ >= 0) &&
               (parameters.ACTIVE_MODE[0] || parameters.ACTIVE_MODE[1] || parameters.ACTIVE_MODE[2] || parameters.ACTIVE_MODE[3]) &&
               ( (parameters.PGA <= 2) || (parameters.PGA == 4) || (parameters.PGA == 8) || (parameters.PGA == 16) );

  if(state == false)
  {
    errorMessage = "Error PMU_ADS1115: One or some parameters are not correct.";
    return false;
  }

  return true;
}
