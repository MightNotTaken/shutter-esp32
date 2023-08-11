#ifndef PYRO_H__
#define PYRO_H__
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 18

namespace Pyro {
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sensors(&oneWire);
  float C;
  uint32_t updationDuration = 1000;
  uint32_t nextRead = 0;
  
  void begin() {
    sensors.begin();
  }

  bool update() {
    if (millis() < nextRead) {
      return C != DEVICE_DISCONNECTED_C;
    }
    nextRead = millis() + updationDuration;
    sensors.requestTemperatures();
    C = sensors.getTempCByIndex(0);
    return C != DEVICE_DISCONNECTED_C;
  }

  float getTemperature() {
    return C;
  }

  void show() {
    Serial.print("Temperature: ");
    Serial.println(C);
  }

  void setUpdationInterval(uint32_t duration) {
    updationDuration = duration;
  }
};
#endif

