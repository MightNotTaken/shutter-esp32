#ifndef SENSOR_H__
#define SENSOR_H__

class Sensor {
  uint8_t GPIO;
  int32_t count;
  uint32_t threshold;
public:
  Sensor(uint8_t GPIO, uint32_t threshold = 200) {
    this->count = 0;
    this->GPIO = GPIO;
    this->threshold = threshold;
  }

  void begin() {
    pinMode(this->GPIO, INPUT);
  }

  void update() {
    this->count += digitalRead(this->GPIO) == LOW ? 1 : -1;
    if (this->count > this->threshold * 1.2) {
      this->count = this->threshold * 1.2;
    }
    
    if (this->count < 0) {
      this->reset();
    }
  }

  void reset() {
    this->count = 0;
  }

  void show() {
    Serial.print("count: ");
    Serial.println(this->count);
  }

  bool available() {
    return this->count > this->threshold;
  }

  int32_t getCount() {
    return this->count;
  }
};
#endif