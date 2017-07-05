/*
    A simple 1-d oscilliscope: scan all the channels, setting the PWM output
    value to 4x the analog pin 0 value (0 - 1024 * 4 = 4096).  The value will
    fade to zero as the channels keep scanning.

    See the BasicUse example for hardware setup.

    Alex Leone <acleone ~AT~ gmail.com>, 2009-02-03 */

#include "SparkFun_Tlc5940.h"
#include "tlc_fades.h"

// which analog pin to use
#define ANALOG_PIN      0

// how many millis to strobe over all the LEDs

TLC_CHANNEL_TYPE channel;

void setup()
{
  Tlc.init();
}

void loop()
{
  // how many millis to strobe over all the LEDs
  static uint16_t scope_period = 1000 * Tlc.num_tlcs;
  static uint32_t led_period = scope_period / (Tlc.num_tlcs * 16);
  
  uint32_t lastMillis = millis();
  tlc_addFade(channel,                      // led channel
              analogRead(ANALOG_PIN) * 4,   // start fade value (0-4095)
              0,                            // end fade value (0-4095)
              lastMillis + 2,               // start millis
              lastMillis + (uint16_t) scope_period / 4  // end millis
             );
  if (channel++ == Tlc.num_tlcs * 16) {
    channel = 0;
  }
  uint32_t currentMillis;
  do {
    currentMillis = millis();
    tlc_updateFades(currentMillis);
  } while (currentMillis - lastMillis <= led_period);
}
