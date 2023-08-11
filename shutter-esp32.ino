#include "mac.h"
#include "database.h"
#include "BLE.h"
#include "pyro.h"
#include "Sensor.h"
#include "config.h"


Sensor IR(IR_SENSOR_PIN);
Sensor vibrator(VIBRATOR_PIN);

void setup() {
  Serial.begin(115200);
  MAC::load();
  Database::begin();
  BLE_System::begin();
  Pyro::begin();
}

void loop() {
  BLE_System::checkConnection();
  Pyro::update();
  IR.update();
  vibrator.update();
  // if (BLE_System::deviceConnected) {
  //   BLE_System::write(String(random(1, 30)));
  // }

  static uint32_t start = millis();
  if (millis() - start > 500) {
    start = millis();
    IR.show();
    vibrator.show();
    Pyro::show();
  }
}
