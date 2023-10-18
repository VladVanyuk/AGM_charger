
#include "includes.h"

#include "pins.h"
#include "timers.h"
#include "flags.h"

#include "display.h"
#include "charge_functions.h"



void setup()
{
  WiFi.mode(WIFI_OFF);
  

  pinsSetup();
  displaySetup();
  
}

void loop()
{
  currentTime = millis();

  display.drawFastHLine(0, 13, 128, WHITE);
  display.display();

  if (digitalRead(ACC_IN_PIN) == HIGH)
  {
    flags.AC = true; // 230vac availiable  run program when AC mains 230VAC are availiable
  }else{
    charged();
    STAGE_FLAGS = {};

    if (currentTime - previousTime >= EVENT_INTERVAL_ONE_SEC)
    {
      calc_SOC_voltage();
      display_SOC_voltage();
      previousTime = currentTime;
    }
    flags.AC = false;                     // reset AC flag
    analogWrite(CHARGE_PUMPIN_PIN, 0); // charge pump
  }
    

  /////////////////////////////////////////////////////////////////////
  
  print_source_detector();
  bat_value();

  if (flags.CSR == false)
  {
    if (currentTime - previousTime >= EVENT_INTERVAL_ONE_SEC)
    {
      calc_SOC_voltage();
      display_SOC_voltage();
      previousTime = currentTime;
    }
  }

  charge_handler();
  
}
