# Speed Sensor
## 1. Hardware
**Arduino:** Receive analog Input of Speed Sensor, count pulses and calculate round per minute (rpm).

**Speed Sensor LM393:** This sensor is used to measure the speed of motors. It uses an encoder disk with holes (here: 20), which pass through a light barrier. This disk is normally fixed to the shaft of the motor. The light barrier can count the holes of the encoder disk so that the rotations can be counted. **[[1]](https://joy-it.net/files/files/Produkte/SEN-Speed/SEN-Speed-Manual-20201015.pdf)**

**RaspberryPi:** Receive raw rpm data and calculate speed data.

The encoder disk is attached onto the car wheels, which needs to be considered for the speed calculation. A better solution would be to mount the encoder disk to the shaft of the motor.

## 2. Wiring
Speed Sensor LM393 -> Arduino Uno ->CAN Shield MCP2515 -> RasperryPi 4
| SpeedSensor | Arduino Uno | Can Shield MCP2515  | 
|--|--|--|
| GND | GND | GND | 
| 5V | 5V | VCC | 
| A0 | D2 |  | 
| D0 | - |  | 
|  | D3 | INT* | 
|  | D13 | SCK | 
|  | D11 | SI | 
|  | D12 | SO| 
|  | D10 | CS | 
\* INT (Interrupt) is not needed here
## 3. RPM Calculation
The Speed Sensor will send an analog signal every time the holes in the disc pass the light barrier. This signals will be counted in the arduino as an interrupt process. For this example 20 rising edges will be one turn. So the basic formula is:
$$
turns = \frac {pulses} {20}
$$
To get rounds per minute (rpm) we need to consider the time frame. As sampling rate 1/100 milliseconds is used, which results in 10 measurements per second. To get the rounds per minute we need the following formula with the units transformation form milliseconds to minutes:\
*rpm* - rounds per minute in [1/min]
$$
rpm = \frac {pulses} {20}* SampleRate *\frac {1000ms} {1s}*\frac {60s} {1min}
$$
## 4. Speed Calculation
For speed calculation the diameter of the car wheel and encoder disk is needed.

*d1* - Diameter of encoder disk\
*d2* - Diameter of car wheels\
*rpm* - rounds per minute
*speed* - in [m/min]
$$
speed = rpm * \frac{d1}{d2} * \frac{d2* \pi}{1000mm}
$$
## 5. Application
The arduino only calculates the RPM and sends this data via CAN bus to the RaspberryPi, which calculates the speed.
## 6. Problems & Solutions
The data fluctuates a lot which results in jumpy display of data instead of a smooth animation. This can be solved with lower SampleRate or averaging the data.
### <u>Source</u>

 - [[1] Speed Sensor LM393 Guideline](https://joy-it.net/files/files/Produkte/SEN-Speed/SEN-Speed-Manual-20201015.pdf)
 - [[2] Arduino Uno Pinout](https://www.circuito.io/blog/arduino-uno-pinout/)
 - [[3] Can Shield MCP2515](https://www.electronicshub.org/arduino-mcp2515-can-bus-tutorial/)

