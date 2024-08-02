#include <ArduinoBLE.h>
#include "Arduino_LED_Matrix.h"   // LED_Matrix library einbinden
#include "frames.h"               // Header-Datei mit benutzerdefinierten Frames einbinden

BLEService ledService("19b10000-e8f2-537e-4f6c-d104768a1214"); // Custom service UUID
BLEByteCharacteristic switchCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite);

ArduinoLEDMatrix matrix;          // Instanz der ArduinoLEDMatrix-Klasse erstellen

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("Arduino");
  BLE.setAdvertisedService(ledService);

  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);

  switchCharacteristic.writeValue(0);

  BLE.advertise();
  Serial.println("BLE LED Peripheral advertising as ArduinoLED");

  matrix.begin();  // LED-Matrix initialisieren
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {
          showRight();
        } else {
          showLeft();
        }
      }
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void showRight() {
  Serial.println("Displaying Right");
  matrix.loadFrame(Right);  // Lade und zeige das Herz-Frame
}

void showLeft() {
  Serial.println("Displaying Left");
  matrix.loadFrame(Left);  // Lade und zeige das Left-Frame
}
