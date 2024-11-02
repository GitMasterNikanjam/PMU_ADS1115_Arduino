#include "PMU_ADS1115.h"

PMU_ADS1115* PMU_ADS1115::_instances = {nullptr};

void _ADS_adsReady(void)
{
  PMU_ADS1115::_instances->_ads_flag = true;
}

PMU_ADS1115::PMU_ADS1115(uint8_t address)
{
  parameters.ADDRESS = address;
  ADS = ADS1115(parameters.ADDRESS);

  parameters.DATA_RATE = 4;
  parameters.RDY_PIN = -1;

  _ads_exist = false;

}

PMU_ADS1115::~PMU_ADS1115()
{

}

bool PMU_ADS1115::init(void)
{
  _funPointer = _ADS_adsReady;

  if(ADS.isConnected())
  {
    _ads_exist = true;
    ADS.begin();
    ADS.setGain(0);        // 6.144 volt --> max voltage read
    ADS.setDataRate(parameters.DATA_RATE); 
      
    // SET ALERT RDY PIN
    ADS.setComparatorThresholdHigh(0x8000);
    ADS.setComparatorThresholdLow(0x0000);
    ADS.setComparatorQueConvert(0);
    
    // SET INTERRUPT HANDLER TO CATCH CONVERSION READY
    pinMode(parameters.RDY_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(parameters.RDY_PIN), _funPointer, RISING);

    ADS.setMode(0);               // Continuse mode
    ADS.readADC(0);     // trigger first read
  }
  else
  {
    _ads_exist = 0;
    // #ifdef DEBUG_ENA
    //   Serial.println("ADS Sensor Not Connected!");
    // #endif
  }
  return true;
}

//   void handleConversion()
//   {
//     if (_ads_flag)
//     {
//       // save the value
//       ADS_val[ADS_channel] = ADS_EXT.getValue();
//       switch(ADS_channel)
//       {
//       case 0:
//         amper_battery=abs(1000.0*ADS_val[0]/ADS_INB_RAW_RANGE*ADS_INB_MAX_VOLTAGE - ACS758_OFFSET_GAIN*voltage_board)*ACS758_SENSITIVITY;
//       break;
//       case 1:
//         voltage_battery=1000.0*ADS_val[1]/ADS_INB_RAW_RANGE*ADS_INB_MAX_VOLTAGE*VOLTAGE_BATTERY_SCALE;
//       break;
//       case 2:
//         voltage_board=1000.0*ADS_val[2]/ADS_INB_RAW_RANGE*ADS_INB_MAX_VOLTAGE*VOLTAGE_BOARD_SCALE;
//       break;
//       }
//       // request next channel
//       ADS_channel++;
//       if (ADS_channel >= 3) ADS_channel = 0;
//       ADS_EXT.readADC(ADS_channel);
//       _ads_flag = false;
//     }
//   }

