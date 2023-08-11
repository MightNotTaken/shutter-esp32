#include "database.h"
#include "BLE.h"
#include "pyro.h"
#include "Sensor.h"
#include "config.h"


Sensor IR(IR_SENSOR_PIN);
Sensor vibrator(VIBRATOR_PIN);

void setup() {
  Serial.begin(115200);
  Database::begin();
  BLE_System::begin();
  Pyro::begin();
  IR.begin();
  vibrator.begin();
}

static uint32_t start = millis();
void loop() {
  BLE_System::checkConnection();
  Pyro::update();
  IR.update();
  vibrator.update();
  if (BLE_System::deviceConnected) {
    BLE_System::write(String(random(1, 30)));
  }
  

  if (millis() - start > 500) {
    start = millis();
    IR.show();
    vibrator.show();
    Pyro::show();
  }
}
