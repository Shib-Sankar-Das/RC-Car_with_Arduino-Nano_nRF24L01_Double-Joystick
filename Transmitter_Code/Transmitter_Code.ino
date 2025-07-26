#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);  // CE, CSN

const byte address[6] = "00001";
char dataToSend[32];

// Joystick analog pin assignments (based on image)
const int joy1_Y = A0;  // Rudder
const int joy1_X = A1;  // Throttle
const int joy2_Y = A2;  // Aileron
const int joy2_X = A3;  // Elevator

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();  // Transmitter mode
  Serial.println("Transmitter Initialized ✅");
}

void loop() {
  int val1X = analogRead(joy1_X);
  int val1Y = analogRead(joy1_Y);
  int val2X = analogRead(joy2_X);
  int val2Y = analogRead(joy2_Y);

  // We'll only use one stick: Right stick for direction (as before)
  // If needed you can map other stick later for more functions
  // Format: "X,Y" (from right joystick)
  sprintf(dataToSend, "%d,%d", val2X, val1Y);
  radio.write(&dataToSend, sizeof(dataToSend));

  Serial.print("Sent: ");
  Serial.println(dataToSend);

  delay(100);  // Adjust as needed
}
