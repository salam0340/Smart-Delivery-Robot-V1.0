# Line Following Robot with Smart Delivery Stops


## 🚀 Overview

An autonomous robot built with Arduino Uno that follows a black line to deliver food orders from kitchen to tables and home-delivery points. Equipped with smart delivery stops (3s at tables, 5s at homes), ultrasonic obstacle avoidance, and an automatic return-to-base routine.

---

## ✨ Features

- **Line Tracking:** Dual IR sensors for precise path following
- **Obstacle Avoidance:** Ultrasonic sensor for real-time detection
- **Smart Delivery Stops:** Configurable pauses (3s at tables, 5s at homes)
- **Automatic Return:** Self-guided return to kitchen base
- **Modular Design:** Breadboard-based wiring and open-source code

---

## 🛠️ Hardware Requirements

| Component                 | Quantity |
|---------------------------|---------:|
| Arduino Uno               |        1 |
| IR Reflectance Sensors    |        2 |
| Ultrasonic Sensor (HC-SR04)|       1 |
| L298N Motor Driver        |        1 |
| SG90 Servo Motor          |        1 |
| Dual DC Gear Motors + Wheels |    2 |
| Mini Breadboard           |        1 |
| On/Off Switch             |        1 |
| Dual-Cell Battery Holder  |        1 |
| 3.6V Rechargeable Batteries |     2 |
| Jumper Wires (M–M, M–F, F–F)| multiple |

---

## 💻 Software Requirements

- **Arduino IDE** (v1.8.XX or higher)
- **Libraries:**
  - `Servo.h`
  - `Keypad.h`

---

## ⚙️ Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/salam0340/Smart-Delivery-Robot-V1.0.git
   cd Smart-Delivery-Robot-V1.0
   ```
2. **Open the Arduino project**
   - Launch Arduino IDE and open `SmartDeliveryStops.ino`.
3. **Install required libraries**
   - Use Library Manager to install `Keypad` and confirm `Servo` is available.
4. **Wiring setup**
   - Follow the `Circuit Diagram` for connections on the breadboard.
5. **Upload firmware**
   - Select `Arduino Uno` board and COM port, then click **Upload**.

---

## 🚀 Usage

1. Power on the robot using the switch.
2. Place it at the starting line on your black track.
3. The robot will begin line-following, pause at delivery stops, and return automatically.
4. Monitor debug output via USB Serial (9600 bps).

---

## ✅ Testing

- **Line Tracking:** Test on straight, curved, and S-shaped tracks under different lighting.
- **Obstacle Avoidance:** Place obstacles at various points to verify halting response.
- **Delivery Stops:** Confirm 3s and 5s pauses with a stopwatch.
- **Return Routine:** Run multiple cycles to ensure >95% success rate.

---

## 🔮 Future Enhancements

- Wireless communication (Bluetooth/Wi-Fi)
- Vision-based navigation (camera or LIDAR)
- Autonomous charging dock
- Multi-robot coordination
- Mobile app for order dispatch and monitoring

---

## 📜 License

This project is released under the MIT License. See [LICENSE](LICENSE) for details.

---

## 🤝 Contributing

Contributions are welcome! 

---

*Developed by Md Abdus Salam, Md. Tanzin Ahmed Shohag, Mst. Farhana Akter, and Jahanara Perven Juthy — Khwaja Yunus Ali University*
