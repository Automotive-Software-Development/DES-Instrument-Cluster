/*
  Speedsensor will get rpm to send via CAN Bus to RaspberryPI
  Speedsensor counts pulses per turn and calculate rpm
  
  CAN Libary
  https://github.com/codeljo/AA_MCP2515
*/

#include "AA_MCP2515.h"

// CAN Bus: modify CAN_BITRATE, CAN_PIN_CS(Chip Select) pin, and CAN_PIN_INT(Interrupt) pin as required.
const CANBitrate::Config CAN_BITRATE = CANBitrate::Config_8MHz_500kbps;
const uint8_t CAN_PIN_CS = 10;
const int8_t CAN_PIN_INT = 3;                                           //InterruptPin for Can (not used)

CANConfig config(CAN_BITRATE, CAN_PIN_CS, CAN_PIN_INT);                 //Intialize CAN Controler with configs
CANController CAN(config);

uint8_t data[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };    //Sampledata

// Speedsensor
int encoder_pin = 2;                  // The Interruptpin the encoder is connected           
unsigned int rpm;                     // rpm Round per Minute
volatile byte pulses;                 // number of pulses
unsigned long timeold;                // old Time variable
unsigned int pulsesperturn = 20;      // The number of pulses per revolution, depends on your index disc (here 20)

 void counter()                       //Count the pulses
 {
      pulses++;                       //Update pulse counter
 }


void setup() {
  Serial.begin(115200);

  while(CAN.begin(CANController::Mode::Normal) != CANController::OK) {            //Start connection
    Serial.println("CAN begin FAIL - delaying for 1 second");
    delay(1000);
  }
  Serial.println("CAN begin OK");
  
   pinMode(encoder_pin, INPUT);
   
   //Interrupt 0 is digital pin 2, so that is where the IR encoder  detector is connected
   //Triggers on FALLING (change from HIGH to LOW)
   attachInterrupt(0, counter, FALLING);
   // Initialize variables
   pulses = 0;
   rpm = 0;
   timeold = 0;
}

void loop() {

   if (millis() - timeold >= 100){  /*Uptade every 100 millisecond, this will be equal to reading frecuency (Hz).*/
 
  //Don't process interrupts during calculations
   detachInterrupt(0);
   //Note that this would be 60*100/(millis() - timeold)*pulses if the interrupt happened once per revolution
   rpm = (60 * 100 / pulsesperturn )/ (millis() - timeold)* pulses;
   timeold = millis();
   pulses = 0;
   
   CANFrame frame(0x0, rpm, sizeof(rpm));       //Create Can Frame to send
   CAN.write(frame);                            //Send Can Frame
   frame.print("CAN TX");
   
   //Write RPM out to serial port
   Serial.print("RPM = ");
   Serial.println(rpm,DEC);                     //Print as decimal Number
   
   //Restart the interrupt processing
   attachInterrupt(0, counter, FALLING);
   }

}
