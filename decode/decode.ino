/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols are active.
 */
//#define DECODE_NEC 1

#include <IRremote.h>

int IR_RECEIVE_PIN = 2;

void setup()
{
    Serial.begin(9600);
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ __TIME__ "\r\nUsing library version " VERSION_IRREMOTE));

    /*
     * Start the receiver, enable feedback LED and take LED feedback pin from the internal boards definition
     */
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);

    Serial.print(F("Ready to receive IR signals at pin "));
    Serial.println(IR_RECEIVE_PIN);
}

void loop()
{
    /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
     */
    delay(1000);
    Serial.println("Print a short summary of received data raw formatted ");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    IrReceiver.resume(); // Enable receiving of the next value
    if (IrReceiver.decode())
    {

        // Print a short summary of received data
        Serial.println("Print a short summary of received data formatted to serial");
        IrReceiver.printIRResultShort(&Serial);
        Serial.println("Print a short summary of received data raw");
        IrReceiver.printIRResultRawFormatted(&Serial, true);
        Serial.println("Print a short summary of received data raw formatted ");
        Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        if (IrReceiver.decodedIRData.protocol == UNKNOWN)
        {
            // We have an unknown protocol here, print more info
            IrReceiver.printIRResultRawFormatted(&Serial, true);
        }
        Serial.println();

        /*
         * !!!Important!!! Enable receiving of the next value,
         * since receiving has stopped after the end of the current received data packet.
         */
        IrReceiver.resume(); // Enable receiving of the next value

        /*
         * Finally, check the received data and perform actions according to the received command
         */
        if (IrReceiver.decodedIRData.command == 0x10)
        {
            // do something
        }
        else if (IrReceiver.decodedIRData.command == 0x11)
        {
            // do something else
        }
    }
}
