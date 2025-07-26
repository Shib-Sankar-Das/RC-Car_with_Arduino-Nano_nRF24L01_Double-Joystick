# 🚗 RC Car Project - Wireless Control System using Arduino Nano, NRF24L01, and L298N Motor Driver

## 📋 Project Overview

This project enables wireless control of an RC car using two joysticks connected to a transmitter (Arduino Nano + NRF24L01) and a receiver setup that drives motors using an L298N motor driver. Communication between the transmitter and receiver is achieved via NRF24L01 modules operating at 2.4GHz frequency.

The system consists of two main components:

1. **🛰️ Transmitter Unit**: A handheld controller with dual joysticks for sending control commands
2. **🛺 Receiver Unit**: Mounted on the RC car to receive commands and control the motors

![final S2](https://github.com/user-attachments/assets/3a34ae7d-4e74-48f6-9ce6-bdced0706833)


## 🔌 Hardware Components Required

### 🛰️ Transmitter Side
- Arduino Nano (1x)
- NRF24L01 PA/LNA Wireless Module (1x) with antenna
- Dual-axis Joystick Modules (2x)
- 7.4V 450mAh Li-Po Battery (1x)
- 100µF Capacitor (for NRF stability)
- Connecting wires
- Breadboard or custom PCB
- Switch for power control

### 🛺 Receiver Side
- Arduino Nano (1x)
- NRF24L01 Wireless Module (1x)
- L298N Motor Driver Module (1x)
- LM2596 Buck Converter Module (1x) - for voltage regulation
- AMS1117 3.3V Regulator Module (1x) - for NRF power
- 2 × 12V 300RPM DC Motors (for left and right wheels)
- 10µF Capacitor (for AMS1117 stability)
- 8V-12V Power Supply (Battery pack recommended)
- Connecting wires
- Chassis/Frame for the car
- Heat sinks for LM2596 and L298N (recommended)


https://github.com/user-attachments/assets/7dae56fb-6ab8-4291-bbf7-26cefedcaa53


## 🔌 Circuit Diagrams Analysis

### 📤 Transmitter Circuit (Transmitter-Diagram.jpg)

The transmitter circuit includes:

**Power Supply:**
- 7.4V Li-Po battery powers the Arduino Nano and NRF module
- Direct power connection to Arduino Nano VIN pin
- Regulated 3.3V output for NRF24L01 module

**Arduino Nano Connections:**
- **NRF24L01 Module:**
  - VCC → 3.3V (⚠️ NEVER connect to 5V)
  - GND → GND
  - CE → Digital Pin 9
  - CSN → Digital Pin 10
  - SCK → Digital Pin 13
  - MOSI → Digital Pin 11
  - MISO → Digital Pin 12

- **Joystick 1 (Left Stick):**
  - VCC → 5V
  - GND → GND
  - X-axis → Analog Pin A1 (Throttle)
  - Y-axis → Analog Pin A0 (Rudder)

- **Joystick 2 (Right Stick):**
  - VCC → 5V
  - GND → GND
  - X-axis → Analog Pin A3 (Elevator)
  - Y-axis → Analog Pin A2 (Aileron)

**Stability Components:**
- 100µF capacitor across NRF24L01 VCC-GND for power stability

![Transmitter-Diagram](https://github.com/user-attachments/assets/e4640840-e326-499a-9ea6-0826adf74428)


### 📥 Receiver Circuit (Receiver-Diagram.jpg)

The receiver circuit includes:

**Power Management System:**
- External 8V-12V input to **LM2596 Buck Converter**
- LM2596 output regulated to 5V for Arduino Nano (via VIN pin)
- **AMS1117 3.3V Regulator** powered from 5V line for NRF module
- 10µF capacitor on AMS1117 input for stability

**Arduino Nano Connections:**
- **NRF24L01 Module:**
  - VCC → 3.3V (from AMS1117 output)
  - GND → Common ground
  - CE → Digital Pin 7
  - CSN → Digital Pin 8
  - SCK → Digital Pin 13
  - MOSI → Digital Pin 11
  - MISO → Digital Pin 12

- **L298N Motor Driver:**
  - IN1 → Digital Pin 2 (Left Motor Direction)
  - IN2 → Digital Pin 3 (Left Motor Direction)
  - ENA → Digital Pin 5 (Left Motor Speed - PWM)
  - IN3 → Digital Pin 4 (Right Motor Direction)
  - IN4 → Digital Pin 9 (Right Motor Direction)
  - ENB → Digital Pin 6 (Right Motor Speed - PWM)
  - VCC → 5V (Logic power from LM2596)
  - 12V → External power supply (Motor power)
  - GND → Common ground

- **Motors:**
  - Left Motor → OUT1 & OUT2 of L298N
  - Right Motor → OUT3 & OUT4 of L298N

![Receiver-Diagram](https://github.com/user-attachments/assets/b94fff4a-e7c4-40f3-9b75-066a3f69b75c)


## 🧠 How It Works

### Communication Protocol
- Uses NRF24L01 2.4GHz wireless modules for bidirectional communication
- Data transmission format: "X,Y" (comma-separated values)
- Communication address: "00001"
- Transmission rate: Every 100ms
- Operating range: Up to 100 meters (with PA/LNA module)

### Control Logic
The system uses joystick inputs for comprehensive car control:
- **Y-axis (Forward/Backward):**
  - Value > 756: Forward movement
  - Value < 556: Backward movement
  - Values between 556-756: Dead zone (no movement)

- **X-axis (Turning):**
  - Value < 545: Turn right
  - Value > 745: Turn left
  - Values between 545-745: Dead zone (straight movement)

### Movement Patterns
1. **Forward**: Both motors rotate forward at controlled speed
2. **Backward**: Both motors rotate backward at controlled speed
3. **Pivot Right**: Left motor forward, right motor backward/stopped
4. **Pivot Left**: Right motor forward, left motor backward/stopped
5. **Forward + Turn**: Differential steering while moving forward
6. **Backward + Turn**: Differential steering while moving backward
7. **Speed Control**: PWM signals (ENA, ENB) control motor speeds

### Advanced Features
- **Variable Speed Control**: PWM-based speed regulation via ENA and ENB pins
- **Signal Timeout**: Motors stop automatically if no signal is received for 500ms
- **Dead Zone**: Prevents unwanted movements due to joystick drift
- **Serial Monitoring**: Real-time status updates for debugging
- **Power Management**: Efficient voltage regulation for stable operation

## 🔧 Assembly Instructions

### ⚠️ CRITICAL PRECAUTIONS & SAFETY GUIDELINES

#### 1. **🧪 Component Pre-testing (MANDATORY)**
Test each component individually before assembly:

**Arduino Nano Testing:**
- Upload and run a simple blink sketch to verify functionality
- Check all digital and analog pins with multimeter

**Joystick Module Testing:**
- Test with basic `analogRead()` sketch
- Verify all 4 analog outputs (X1, Y1, X2, Y2)
- Check voltage levels (should be 0-5V range)

**NRF24L01 Module Testing:**
- Use basic transmitter-receiver test sketches
- Verify 3.3V power requirement (⚠️ NEVER 5V)
- Test communication range and stability

**Motor Testing:**
- Test each motor with direct 12V connection
- Check rotation direction and current draw
- Verify motor specifications (12V, 300RPM)

**L298N Driver Testing:**
- Test with simple HIGH/LOW commands
- Verify PWM speed control functionality
- Check heat generation under load

**Voltage Regulator Testing:**
- LM2596: Verify 5V output from 8-12V input
- AMS1117: Verify 3.3V output from 5V input
- Use multimeter to confirm voltage stability

#### 2. **⚡ Power Supply Safety (CRITICAL)**

**Voltage Level Verification:**
- **NEVER** connect NRF24L01 directly to 5V (will damage module)
- **ALWAYS** use 3.3V regulated supply for NRF modules
- **Double-check** polarity before connecting batteries
- **Verify** voltage levels with multimeter before powering circuits

**Power Connection Sequence:**
1. Connect all signal wires first
2. Connect ground connections
3. Connect 3.3V and 5V supplies
4. **Connect main power supply LAST**
5. Monitor current draw during initial testing

**Battery Safety:**
- Use appropriate gauge wires for high-current connections
- Install fuses in power circuits (recommended: 5A for motor circuit)
- Never exceed component voltage ratings
- Keep spare batteries charged and ready

#### 3. **🔌 Connection Safety & Verification**

**Pre-Power Checks:**
- **Test all connections multiple times** with multimeter
- Verify continuity for all signal paths
- **Check for short circuits** between VCC and GND
- Ensure no loose connections that could cause intermittent faults
- Protect exposed connections with heat shrink tubing or electrical tape

**Wire Management:**
- Use different colored wires for different voltage levels
- Secure all connections with proper soldering or reliable connectors
- Avoid overlapping or loose wires that could cause shorts
- Label all connections for easy troubleshooting

#### 4. **🧊 Thermal Management**

**Heat Sink Requirements:**
- Install heat sinks on LM2596 buck converter
- Install heat sinks on L298N motor driver
- Ensure adequate ventilation around power components
- Monitor component temperatures during extended operation

#### 5. **📦 Capacitor Requirements (MANDATORY)**

**NRF24L01 Stability:**
- **100µF capacitor** across NRF24L01 VCC-GND (prevents brownouts)
- Place capacitor as close as possible to NRF module
- Use ceramic or electrolytic capacitor with appropriate voltage rating

**AMS1117 Stability:**
- **10µF capacitor** on AMS1117 input for voltage stability
- Prevents voltage fluctuations affecting NRF performance

### 📋 Step-by-Step Assembly

#### 🛰️ Transmitter Assembly:
1. **Power Setup**: Connect 7.4V Li-Po battery to Arduino Nano VIN
2. **NRF Module**: Connect NRF24L01 with 100µF capacitor for stability
3. **Joysticks**: Connect both joystick modules to respective analog pins
4. **Verification**: Use multimeter to verify all connections
5. **Code Upload**: Upload transmitter code and test joystick readings
6. **Communication Test**: Test wireless transmission using serial monitor
7. **Range Test**: Verify communication range and signal strength

#### 🛺 Receiver Assembly:
1. **Power Regulation Setup**:
   - Connect LM2596 buck converter (8-12V input → 5V output)
   - Connect AMS1117 3.3V regulator (5V input → 3.3V output)
   - Add 10µF capacitor to AMS1117 input
2. **Arduino Power**: Connect 5V from LM2596 to Arduino Nano VIN
3. **NRF Module**: Connect NRF24L01 to 3.3V from AMS1117
4. **Motor Driver**: Connect L298N inputs to Arduino digital pins
5. **Motor Power**: Connect 8-12V supply to L298N motor power input
6. **Motors**: Connect DC motors to L298N outputs
7. **Safety Check**: Test each motor individually before final assembly
8. **Code Upload**: Upload receiver code and test signal reception
9. **System Integration**: Test complete system functionality

### ✅ Final Pre-Power Checklist

**Before powering the complete system, verify:**

- [ ] **Individual Component Testing**: All components tested separately
- [ ] **Voltage Verification**: All voltage levels confirmed with multimeter
- [ ] **Connection Integrity**: All connections tested for continuity
- [ ] **Short Circuit Check**: No shorts between VCC and GND
- [ ] **Capacitor Installation**: Stability capacitors properly installed
- [ ] **Heat Sink Installation**: Heat sinks mounted on power components
- [ ] **Wire Gauge**: Appropriate wire sizes for current requirements
- [ ] **Polarity Check**: Battery polarity double-checked
- [ ] **Ground Connections**: All grounds properly connected
- [ ] **Secure Connections**: All connections tight and properly insulated

## Code Features

### Transmitter Code (`Transmitter_Code.ino`)
- Reads analog values from joysticks
- Formats data as comma-separated string
- Transmits data via nRF24L01
- Serial output for debugging

### Receiver Code (`Receiver_Code.ino`)
- Receives and parses joystick data
- Implements dead zone for stable control
- Controls motor directions via L298N
- Safety timeout mechanism
- Comprehensive serial debugging

## 🚨 Troubleshooting Guide

### Common Issues & Solutions:

#### 1. **📡 No Communication Between Modules**:
**Symptoms**: No data received, timeout messages
**Solutions**:
- Check NRF24L01 connections and ensure 3.3V power (⚠️ NOT 5V)
- Verify both modules use same communication address ("00001")
- Ensure proper grounding between transmitter and receiver
- Add/check 100µF capacitor across NRF power lines
- Test modules individually with basic sketches
- Check antenna orientation and range

#### 2. **🔋 Power Supply Issues**:
**Symptoms**: Erratic behavior, module resets, brownouts
**Solutions**:
- Verify LM2596 output voltage (should be 5.0V ±0.1V)
- Check AMS1117 output voltage (should be 3.3V ±0.1V)
- Test battery voltage under load
- Ensure adequate current capacity for all components
- Check for voltage drops across connections

#### 3. **🚗 Motors Not Working or Erratic Movement**:
**Symptoms**: Motors don't respond, wrong direction, inconsistent speed
**Solutions**:
- Verify L298N connections and power supply
- Test motors with direct 12V connection
- Check PWM signals on ENA and ENB pins
- Verify IN1-IN4 digital signal levels
- Ensure motor power supply can handle current requirements
- Check for loose motor connections

#### 4. **🕹️ Joystick Calibration Issues**:
**Symptoms**: Car moves without joystick input, wrong directions
**Solutions**:
- Calibrate joystick center values using serial monitor
- Adjust dead zone parameters in code
- Check joystick power supply (5V)
- Test individual joystick analog readings
- Verify joystick wiring connections

#### 5. **📶 Signal Loss or Intermittent Connection**:
**Symptoms**: Frequent timeouts, loss of control
**Solutions**:
- Reduce distance between modules for testing
- Check antenna orientation (parallel antennas work best)
- Verify stable power supply to both modules
- Eliminate sources of 2.4GHz interference
- Increase transmission power level in code

#### 6. **🔥 Overheating Components**:
**Symptoms**: Components hot to touch, thermal shutdown
**Solutions**:
- Install heat sinks on LM2596 and L298N
- Ensure adequate ventilation
- Check for short circuits causing excessive current
- Verify component current ratings vs. actual draw
- Consider active cooling for extended operation

## 🧪 Comprehensive Testing Procedure

### Phase 1: Individual Component Testing
1. **Arduino Nano Testing**: Upload blink sketch, verify all pins
2. **Power Regulation Testing**: Verify LM2596 and AMS1117 outputs
3. **Joystick Testing**: Test analog readings and full range motion
4. **NRF Module Testing**: Basic ping-pong communication test
5. **Motor Testing**: Direct power connection and rotation verification
6. **L298N Testing**: PWM speed control and direction verification

### Phase 2: Subsystem Testing
7. **Transmitter Testing**: Complete joystick data transmission
8. **Receiver Testing**: Data reception and parsing verification
9. **Motor Control Testing**: Joystick to motor movement correlation

### Phase 3: Integration Testing
10. **Communication Range Testing**: Test at various distances
11. **Complete System Testing**: Full functionality verification
12. **Stress Testing**: Extended operation under various conditions
13. **Battery Life Testing**: Monitor power consumption and runtime
14. **Response Time Testing**: Measure control latency and responsiveness

### Phase 4: Performance Optimization
15. **Signal Strength Optimization**: Antenna positioning and power levels
16. **Control Sensitivity Tuning**: Dead zone and response curve adjustment
17. **Speed Control Calibration**: PWM values for optimal performance

## Safety Guidelines

- Always disconnect power when making connections
- Use appropriate fuses in power circuits
- Never exceed component voltage ratings
- Keep spare components for critical parts
- Document all modifications
- Test in safe, open area during initial trials

## 💡 Suggestions for Improvement & Advanced Features

### Hardware Enhancements:
- **OLED Display**: Add battery status and signal strength feedback
- **GPS Module**: Integrate location-based control and tracking
- **Camera Module**: Add FPV (First Person View) capability
- **Servo Motors**: Add steering mechanism for better control
- **Encoders**: Implement precise speed and position feedback
- **IMU Sensor**: Add gyroscope for stability control

### Software Upgrades:
- **ESP32 Migration**: Upgrade to ESP32 for Wi-Fi + Bluetooth support
- **Mobile App Control**: Develop smartphone application interface
- **Autonomous Mode**: Implement obstacle avoidance algorithms
- **Telemetry System**: Real-time data logging and monitoring
- **PID Control**: Advanced motor speed and direction control
- **Multiple Car Support**: Multi-channel communication system

### Performance Optimizations:
- **Variable Speed Control**: Smooth acceleration and deceleration
- **Steering Algorithms**: Implement car-like steering behavior
- **Power Management**: Sleep modes and battery optimization
- **Fail-Safe Systems**: Emergency stop and recovery mechanisms

## 📁 Files Structure

```
RC-Car-Project/
├── README.md                    # 📖 This comprehensive documentation
├── Transmitter-Diagram.jpg      # 🛰️ Transmitter circuit diagram
├── Receiver-Diagram.jpg         # 🛺 Receiver circuit diagram
├── Transmitter_Code/
│   └── Transmitter_Code.ino     # 📤 Arduino code for transmitter
└── Receiver_Code/
    └── Receiver_Code.ino        # 📥 Arduino code for receiver
```

## 🔧 Developed & Assembled By

**Electronics & Embedded Systems Project**  
*IoT Development | Wireless Communication | Motor Control Systems*

---

## 📄 License & Usage

This project is developed for educational purposes as part of the **Bits2Bytes 2K25** competition. 

**Usage Guidelines:**
- Feel free to modify and enhance the design
- Ensure proper safety measures when building and operating
- Share improvements with the community
- Always test in safe, controlled environments

---

## ⚠️ FINAL SAFETY REMINDER

**🚨 CRITICAL: Always prioritize safety during assembly and operation!**

- ✅ Test each component individually before integration
- ✅ Verify all voltage levels with multimeter
- ✅ Double-check all connections for shorts and proper polarity
- ✅ Use appropriate fuses and protective components
- ✅ Operate in safe, open areas during testing
- ✅ Keep emergency stop procedures ready
- ✅ Monitor component temperatures during operation
- ✅ Use fresh, properly charged batteries
- ✅ Document all modifications and changes

**Remember: A few extra minutes of verification can prevent hours of troubleshooting and potential component damage!**
