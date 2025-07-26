#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);  // CE, CSN pins for nRF24L01
const byte address[6] = "00001";

char receivedData[32] = "";
int xAxis = 645, yAxis = 656;

unsigned long lastSignalTime = 0;
const unsigned long SIGNAL_TIMEOUT = 500;

// Updated Joystick Calibration
const int CENTER_X = 645;
const int CENTER_Y = 656;
const int DEAD_ZONE = 100;  // ±100 tolerance from center

void setup() {
  pinMode(8, OUTPUT);  // in1 - Left Motor
  pinMode(7, OUTPUT);  // in2
  pinMode(4, OUTPUT);  // in3 - Right Motor
  pinMode(3, OUTPUT);  // in4

  Serial.begin(9600);
  Serial.println("✅ Receiver Initialized - Awaiting Data");

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&receivedData, sizeof(receivedData));
    Serial.print("📨 Received: ");
    Serial.println(receivedData);

    char *xStr = strtok(receivedData, ",");
    char *yStr = strtok(NULL, ",");

    if (xStr && yStr) {
      xAxis = atoi(xStr);
      yAxis = atoi(yStr);
      lastSignalTime = millis();
    }
  }

  // Stop motors if no signal
  if (millis() - lastSignalTime > SIGNAL_TIMEOUT) {
    stopMotors();
    Serial.println("⚠️ Signal Lost - Motors Stopped");
    return;
  }

  handleJoystick(xAxis, yAxis);
}

void handleJoystick(int x, int y) {
  bool forward = false;
  bool backward = false;
  bool turning = false;

  // Y-axis → Forward or Backward
  if (y > CENTER_Y + DEAD_ZONE) forward = true;
  else if (y < CENTER_Y - DEAD_ZONE) backward = true;

  // X-axis → Turning
  if (x < CENTER_X - DEAD_ZONE || x > CENTER_X + DEAD_ZONE) turning = true;

  if (forward && !turning) {
    setMotorDir(true, true);
    Serial.println("➡️ Forward");
  } else if (backward && !turning) {
    setMotorDir(false, false);
    Serial.println("⬅️ Backward");
  } else if (turning && !forward && !backward) {
    if (x < CENTER_X - DEAD_ZONE) {
      setMotorDir(true, false);  // Pivot Right
      Serial.println("↪️ Pivot Right");
    } else {
      setMotorDir(false, true);  // Pivot Left
      Serial.println("↩️ Pivot Left");
    }
  } else if (turning && (forward || backward)) {
    if (forward) {
      setMotorDir(true, true);
      Serial.println("🔄 Forward + Turn");
    } else {
      setMotorDir(false, false);
      Serial.println("🔁 Backward + Turn");
    }
  } else {
    stopMotors();
    Serial.println("🛑 Idle");
  }
}

void setMotorDir(bool leftFwd, bool rightFwd) {
  digitalWrite(8, leftFwd ? LOW : HIGH);   // in1
  digitalWrite(7, leftFwd ? HIGH : LOW);   // in2
  digitalWrite(4, rightFwd ? LOW : HIGH);  // in3
  digitalWrite(3, rightFwd ? HIGH : LOW);  // in4
}

void stopMotors() {
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
  digitalWrite(4, LOW);
  digitalWrite(3, LOW);
}
