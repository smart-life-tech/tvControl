/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols are active.
 */
// #define DECODE_NEC 1

#include <IRremote.h>

int IR_RECEIVE_PIN = 2;
IRrecv irrecv(IR_RECEIVE_PIN);

decode_results results;

void dump(decode_results *results)
{
    // Dumps out the decode_results structure.
    // Call this after IRrecv::decode()
    int count = results->rawlen;
    if (results->decode_type == UNKNOWN)
    {
        Serial.print("Unknown encoding: ");
    }
    else if (results->decode_type == NEC)
    {
        Serial.print("Decoded NEC: ");
    }
    else if (results->decode_type == SONY)
    {
        Serial.print("Decoded SONY: ");
    }
    else if (results->decode_type == RC5)
    {
        Serial.print("Decoded RC5: ");
    }
    else if (results->decode_type == RC6)
    {
        Serial.print("Decoded RC6: ");
    }
    else if (results->decode_type == PANASONIC)
    {
        Serial.print("Decoded PANASONIC - Address: ");
       // Serial.print(results->address, HEX);
        Serial.print(" Value: ");
    }
    else if (results->decode_type == LG)
    {
        Serial.print("Decoded LG: ");
    }
    else if (results->decode_type == JVC)
    {
        Serial.print("Decoded JVC: ");
    }
    else if (results->decode_type == SAMSUNG)
    {
        Serial.print("Decoded samsung RC T501: ");
    }
    else if (results->decode_type == WHYNTER)
    {
        Serial.print("Decoded Whynter: ");
    }
    Serial.print(results->value, HEX);
    Serial.print(" (");
    Serial.print(results->bits, DEC);
    Serial.println(" bits)");
    Serial.print("Raw (");
    Serial.print(count, DEC);
    Serial.print("): ");

    for (int i = 1; i < count; i++)
    {
        if (i & 1)
        {
            Serial.print(results->rawbuf[i] , DEC);
        }
        else
        {
            Serial.write('-');
            Serial.print((unsigned long)results->rawbuf[i] , DEC);
        }
        Serial.print(" ");
    }
    Serial.println();
}

void setup()
{
    Serial.begin(9600);
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ __TIME__ "\r\nUsing library version " VERSION_IRREMOTE));

    /*
     * Start the receiver, enable feedback LED and take LED feedback pin from the internal boards definition
     */
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
    irrecv.enableIRIn(); // Start the receiver
    Serial.print(F("Ready to receive IR signals at pin "));
    Serial.println(IR_RECEIVE_PIN);
    Serial.print("Beginning recieve");
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
    // delay(1000);
    // Serial.println("Print a short summary of received data raw formatted ");
    // Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    // IrReceiver.resume(); // Enable receiving of the next value
    if (irrecv.decode(&results))
    {
        Serial.println(results.value, HEX);
        dump(&results);
        Serial.println("Print a short summary of received data formatted to serial");
        IrReceiver.printIRResultShort(&Serial);
        irrecv.resume(); // Receive the next value
    }

}
