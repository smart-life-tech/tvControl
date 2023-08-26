#include <Wire.h>
#include <BH1750.h>   // Light sensor library
#include <IRremote.h> // IR blaster library

#define LIGHT_SENSOR_ADDRESS_FRONT 0x23       // I2C address of front light sensor
#define LIGHT_SENSOR_ADDRESS_BACK 0x24        // I2C address of back light sensor
#define IR_LED_PIN 3                          // Pin connected to IR LED
#define TV_IR_CODE_BRIGHTNESS_UP 0x12345678   // Example IR code for brightness up
#define TV_IR_CODE_BRIGHTNESS_DOWN 0x87654321 // Example IR code for brightness down
int IR_RECEIVE_PIN = 11;
uint16_t tAddress = 1;
BH1750 lightSensorFront(LIGHT_SENSOR_ADDRESS_FRONT);
BH1750 lightSensorBack(LIGHT_SENSOR_ADDRESS_BACK);
IRsend irSender;

int currentBrightness = 50; // Initial TV brightness
int targetBrightness = 50;  // Initial TV brightness
bool adjusting = false;     // Flag to track ongoing adjustments
struct storedIRDataStruct
{
  IRData receivedIRData;
  // extensions for sendRaw
  uint8_t rawCode[RAW_BUFFER_LENGTH]; // The durations if raw
  uint8_t rawCodeLength;              // The length of the code
} sStoredIRData;

void setup()
{
  Serial.begin(9600);
  lightSensorFront.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
  lightSensorBack.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver, enable feedback LED, take LED feedback pin from the internal boards definition

  IrSender.begin(true); // Enable feedback LED,

  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(IR_RECEIVE_PIN);

  Serial.print(F("Ready to send IR signals at pin "));
  Serial.print(IR_SEND_PIN);
}

void loop()
{
  int ambientLightFront = lightSensorFront.readLightLevel();
  int ambientLightBack = lightSensorBack.readLightLevel();

  // Perform your logic to determine target brightness
  // based on ambientLightFront and ambientLightBack
  // ...

  // Perform IR transmission for brightness adjustment
  if (targetBrightness > currentBrightness)
  {
    // irSender.sendNEC(TV_IR_CODE_BRIGHTNESS_UP, 32);
   // irSender.sendSamsung(tAddress, TV_IR_CODE_BRIGHTNESS_UP, 0, false);
    irSender.sendSamsung(TV_IR_CODE_BRIGHTNESS_UP, 0, false);
    currentBrightness = targetBrightness;
  }
  else if (targetBrightness < currentBrightness)
  {
    // irSender.sendNEC(TV_IR_CODE_BRIGHTNESS_DOWN, 32);
    currentBrightness = targetBrightness;
  }

  // Introduce delay to prevent rapid adjustments
  delay(5000);
}

void sendCode(storedIRDataStruct *aIRDataToSend)
{
  if (aIRDataToSend->receivedIRData.protocol == UNKNOWN /* i.e. raw */)
  {
    // Assume 38 KHz
    IrSender.sendRaw(aIRDataToSend->rawCode, aIRDataToSend->rawCodeLength, 38);

    Serial.print(F("Sent raw "));
    Serial.print(aIRDataToSend->rawCodeLength);
    Serial.println(F(" marks or spaces"));
  }
  else
  {

    /*
     * Use the write function, which does the switch for different protocols
     */
    IrSender.write(&aIRDataToSend->receivedIRData, NO_REPEATS);

    Serial.print(F("Sent: "));
    printIRResultShort(&Serial, &aIRDataToSend->receivedIRData);
  }
}