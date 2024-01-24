// import the required library
#include "AA_MCP2515.h"

// TODO: modify CAN_BITRATE, CAN_PIN_CS(Chip Select) pin, and CAN_PIN_INT(Interrupt) pin as required.
const CANBitrate::Config CAN_BITRATE = CANBitrate::Config_8MHz_1000kbps;
const uint8_t CAN_PIN_CS = 10; // pin number to which CS pin of CAN controller is connected
const int8_t CAN_PIN_INT = 3; // pin number to which Interrupt pin of CAN controller is connected

// Create an instance of CANConfig class and pass the parameters
CANConfig config(CAN_BITRATE, CAN_PIN_CS, CAN_PIN_INT);

// Create an instance of CANContoller class and pass the required config
CANController CAN(config);

const int encoder_pin = 2; // encoder pin
const unsigned int pulsesperturn = 20; // Number of wholes in the wheel i.e. number of pulses it can generate in one complete revolution
unsigned int pulses; // To store the number of pulses detected
unsigned long timeold; // To store the timestamp of the last time when the RPM calculation was performed
float rps; // To store the rotations per minute count

// Increases the pulses when the falling edge is detected on the digital pin
void counter() {
    pulses++;
}

void setup() {
    // To initialize the serial communication at baud rate of 115200
    Serial.begin(115200);

    // Repeatedly try to intialize the CAN controller until it succeeds
    while (CAN.begin(CANController::Mode::Normal) != CANController::OK) {
        Serial.println("CAN begin FAIL - delaying for 1 second");
        delay(1000);
    }

    // After successful connection, print this to the serial monitor
    Serial.println("CAN begin OK");

    pinMode(encoder_pin, INPUT);
    attachInterrupt(0, counter, FALLING);
    pulses = 0;
    rps = 0;
    timeold = millis();
}

void loop() {
    // RPM calculation and CAN message transmission every 100 milliseconds
    if (millis() - timeold >= 200) {
        // Disable interrupts during calculations
        detachInterrupt(0);

        // Calculate RPM based on pulses per turn and time elapsed
        rps = (pulses / 20.0) * 5;
        timeold = millis();
        pulses = 0;

        // Transmit RPM data over CAN
        float rpsData[1] = {rps};
        
        // Create a CAN frame to be sent
        CANFrame frame(0x100, rpsData, sizeof(rpsData));

        // Send a CAN frame
        CAN.write(frame);

        // Print a message to the serial monitor
        frame.print("CAN TX");

        // Enable interrupts after calculations
        attachInterrupt(0, counter, FALLING);
    }

    // Simulate updated data from sensor, etc.
    delay(50);
}