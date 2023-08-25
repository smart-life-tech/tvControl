#include <Wire.h>
#include <BH1750.h>  // Light sensor library
#include <IRremote.h> // IR blaster library

#define LIGHT_SENSOR_ADDRESS_FRONT 0x23 // I2C address of front light sensor
#define LIGHT_SENSOR_ADDRESS_BACK 0x24  // I2C address of back light sensor
#define IR_LED_PIN 3 // Pin connected to IR LED
#define TV_IR_CODE_BRIGHTNESS_UP 0x12345678 // Example IR code for brightness up
#define TV_IR_CODE_BRIGHTNESS_DOWN 0x87654321 // Example IR code for brightness down

BH1750 lightSensorFront(LIGHT_SENSOR_ADDRESS_FRONT);
BH1750 lightSensorBack(LIGHT_SENSOR_ADDRESS_BACK);
IRsend irSender;

int currentBrightness = 50; // Initial TV brightness
bool adjusting = false; // Flag to track ongoing adjustments

void setup() {
  Serial.begin(9600);
  lightSensorFront.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
  lightSensorBack.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
}

void loop() {
  int ambientLightFront = lightSensorFront.readLightLevel();
  int ambientLightBack = lightSensorBack.readLightLevel();

  // Perform your logic to determine target brightness
  // based on ambientLightFront and ambientLightBack
  // ...

  // Perform IR transmission for brightness adjustment
  if (targetBrightness > currentBrightness) {
    irSender.sendNEC(TV_IR_CODE_BRIGHTNESS_UP, 32);
    currentBrightness = targetBrightness;
  } else if (targetBrightness < currentBrightness) {
    irSender.sendNEC(TV_IR_CODE_BRIGHTNESS_DOWN, 32);
    currentBrightness = targetBrightness;
  }

  // Introduce delay to prevent rapid adjustments
  delay(5000);
}
