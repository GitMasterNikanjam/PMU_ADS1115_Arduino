#include "PMU_ADS1117.h"




bool PMU_ADS1117::init(void)
{
  // if(ADS.isConnected())
  // {
  //   ads_exist = 1;
  //   ADS.begin();
  //   ADS.setGain(0);        // 6.144 volt --> max voltage read
  //   ADS.setDataRate(ADS_INB_DATA_RATE); 
      
  //   // SET ALERT RDY PIN
  //   ADS.setComparatorThresholdHigh(0x8000);
  //   ADS.setComparatorThresholdLow(0x0000);
  //   ADS.setComparatorQueConvert(0);
    
  //   // SET INTERRUPT HANDLER TO CATCH CONVERSION READY
  //   pinMode(ADS_INB_RDY_PIN, INPUT_PULLUP);
  //   attachInterrupt(digitalPinToInterrupt(ADS_INB_RDY_PIN), ADS_adsReady, RISING);

  //   ADS.setMode(0);               // Continuse mode
  //   ADS.readADC(ADS_channel);     // trigger first read
  // }
  // else
  // {
  //   ads_exist = 0;
  //   // #ifdef DEBUG_ENA
  //   //   Serial.println("ADS Sensor Not Connected!");
  //   // #endif
  // }
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

// void ADS_EXT_adsReady(void)
// {
//   _ads_flag = true;
// }