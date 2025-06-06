#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <ESP32Servo.h>

//Servo myServo;

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
String tagId = "None";
byte nuidPICC[4];
 
void setup(void) 
{
 Serial.begin(115200);
 Serial.println("System initialized");
 //myServo.attach(27);
 nfc.begin();
}
 
void loop() 
{
 readNFC();
}
 
void readNFC() 
{
 if (nfc.tagPresent())
 {
   NfcTag tag = nfc.read();
   tag.print();
   tagId = tag.getUidString();
   //myServo.write(90);
 
    delay(5000);
    //myServo.write(0);

 }
 delay(5000);
}