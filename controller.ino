#include "HomeSpan.h"         // Always start by including the HomeSpan library

struct ATTIC_FAN : Service::Fan {
  SpanCharacteristic *speed;
  SpanCharacteristic *on;

  ATTIC_FAN() : Service::Fan() {
    on = new Characteristic::Active();
    speed = new Characteristic::RotationSpeed(0);
    speed->setRange(0, 100, 50);
  }

  boolean update(){
    Serial.printf("Updating speed to %i, on/off to %i\n", speed->getNewVal(), on->getNewVal());

    if (on->getNewVal() == 0) {
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      return(true);
    }

    Serial.println();
    switch (speed->getNewVal()) {
      case 0:
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        return(true);
      case 50:
        digitalWrite(5, HIGH);
        delay(300);
        digitalWrite(4, LOW);
        return(true);
      case 100:
        digitalWrite(4, HIGH);
        delay(300);
        digitalWrite(5, LOW);
        return(true);
    }
    return(true);
  }
};

void setup() {
  Serial.begin(115200);      // Start a serial connection - this is needed for you to type in your WiFi credentials

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);

  homeSpan.begin(Category::Fans,"Attic Fan");  // Initialize HomeSpan - note the Category has been set to "Fans"
  new SpanAccessory();

  new Service::AccessoryInformation();
    new Characteristic::Identify();
  
  new ATTIC_FAN();
}

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();         // run HomeSpan!
  
} // end of loop()
