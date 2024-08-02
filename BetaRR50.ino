#include <ArduinoBLE.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include "frames.h"

ArduinoLEDMatrix matrix;

const int ledPin = LED_BUILTIN;

BLEService arrowService("19B10010-E8F2-537E-4F6C-D104768A1214"); // BLE Service

BLEByteCharacteristic leftArrowCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLEWrite); // Characteristic for left arrow
BLEByteCharacteristic rightArrowCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLEWrite); // Characteristic for right arrow

unsigned long arrowDisplayStartTime = 0;
bool displayingArrow = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  matrix.begin();

  // Display initial message
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(100);

  const char text[] = "  Beta rr 50   Ready!  ";
  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText(SCROLL_LEFT);

  matrix.endDraw();

  // Begin initialization of BLE
  if (!BLE.begin()) {
    while (1);
  }

  BLE.setLocalName("BetaRR50");
  BLE.setAdvertisedService(arrowService);

  arrowService.addCharacteristic(leftArrowCharacteristic);
  arrowService.addCharacteristic(rightArrowCharacteristic);

  BLE.addService(arrowService);

  leftArrowCharacteristic.writeValue(0);
  rightArrowCharacteristic.writeValue(0);

  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    while (central.connected()) {
      if (leftArrowCharacteristic.written() && leftArrowCharacteristic.value()) {
        matrix.loadFrame(left_frame);
        // Turn off the display
        delay(5000);
        matrix.clear();
      }
      if (rightArrowCharacteristic.written() && rightArrowCharacteristic.value()) {
        matrix.loadFrame(right_frame);
        // Turn off the display
        delay(5000);
        matrix.clear();
        
      }
    }
  }
  
  // Blink the Bluetooth logo if no arrow is displayed
  if (!displayingArrow) {
    matrix.loadFrame(bluetooth_logo);
    delay(1000);  // Pause for 1 second

    // Turn off the display
    matrix.clear();
    delay(1000);
  }
}
