


<div align="center">

<h1>🤖 Mini Bot</h1>
<h3>ROS2 Humble + ESP32 + Micro-ROS | Wireless Differential Drive Robot</h3>

![ROS2](https://img.shields.io/badge/ROS2-Humble-blue?style=for-the-badge&logo=ros)
![ESP32](https://img.shields.io/badge/ESP32-Micro--ROS-red?style=for-the-badge&logo=espressif)
![Platform](https://img.shields.io/badge/Platform-Ubuntu%2022.04-orange?style=for-the-badge&logo=ubuntu)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)
![Arduino](https://img.shields.io/badge/Arduino-IDE%202.x-teal?style=for-the-badge&logo=arduino)

<br/>

> Control a differential-drive robot wirelessly using **ROS2 Humble** on a Linux PC and **Micro-ROS** on an **ESP32** over WiFi UDP. Drive two DC motors via an **L298N H-Bridge** with full keyboard teleop support.

<br/>

[🚀 Quick Start](#-quick-start) • [📐 Wiring](#-wiring--pin-connections) • [💻 Code Overview](#-esp32-code-overview) • [🔧 Troubleshooting](#-troubleshooting) • [📖 References](#-references)

</div>

---

## 📁 Repository Structure

```
mini_bot_ros2_esp32/
├── 📂 arduino/
│   ├── 📂 ESP32_L298N_MicroROS_CmdVel_WiFi/     ← ✅ Main sketch (WiFi/UDP)
│   │   └── ESP32_L298N_MicroROS_CmdVel_WiFi.ino
│   └── 📂 microros_cmd_vel_serial/              ← Serial/USB fallback
│       └── microros_cmd_vel_serial.ino
├── 📂 docs/
│   ├── wiring_diagram.md
│   ├── troubleshooting.md
│   └── led_status.md
├── 📂 scripts/
│   ├── setup_microros_agent.sh    ← One-time setup script
│   ├── run_agent_wifi.sh
│   ├── run_agent_serial.sh
│   └── run_teleop.sh
└── README.md
```

---

## 🛠️ Hardware Requirements

| Component | Specification | Notes |
|-----------|--------------|-------|
| **ESP32 Dev Module** | Any 38-pin ESP32 | Use Espressif board v2.0.2 in Arduino IDE |
| **L298N Motor Driver** | Dual H-Bridge, 2A/ch | 12V input; has onboard 5V regulator |
| **DC Motors × 2** | 3V–12V with gearbox | Match voltage to battery |
| **12V Battery** | Li-Ion or NiMH | Powers L298N & motors |
| **USB Cable** | Micro-USB or USB-C | Flash ESP32 via Arduino IDE |
| **WiFi Router** | 2.4 GHz 802.11 b/g/n | ⚠️ ESP32 does NOT support 5 GHz |
| **Robot Chassis** | 2WD differential | Any standard 2-wheel platform |

---

## 💿 Software Prerequisites

```bash
# ROS2 Humble (Ubuntu 22.04)
sudo apt install ros-humble-desktop

# Build tools
sudo apt install python3-colcon-common-extensions python3-rosdep2

# Teleop keyboard
sudo apt install ros-humble-teleop-twist-keyboard
```

Also required:
- [Arduino IDE 2.x](https://www.arduino.cc/en/software)
- ESP32 board support **v2.0.2** (via Arduino Boards Manager)
- `micro_ros_arduino` library (via Arduino Library Manager)

---

## 🚀 Quick Start

### Step 1 — Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/mini_bot_ros2_esp32.git
cd mini_bot_ros2_esp32
```

### Step 2 — Setup Micro-ROS Agent (One-time)

```bash
chmod +x scripts/setup_microros_agent.sh
./scripts/setup_microros_agent.sh
```

<details>
<summary>📋 What this script does (click to expand)</summary>

```bash
# Creates ~/microros_ws workspace
mkdir -p ~/microros_ws/src
cd ~/microros_ws/src

# Clones micro-ROS-Agent (humble branch)
git clone -b humble https://github.com/micro-ROS/micro-ROS-Agent.git

# Installs dependencies & builds
cd ~/microros_ws
rosdep install --from-paths src --ignore-src -y
colcon build
source install/setup.bash
```

</details>

### Step 3 — Setup Arduino IDE

1. Open Arduino IDE → **File → Preferences**
2. Add to **Additional Board Manager URLs**:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. **Tools → Board → Boards Manager** → search `ESP32` → install **ESP32 by Espressif Systems v2.0.2**
4. **Sketch → Include Library → Manage Libraries** → search `micro_ros_arduino` → install

> 📌 Reference: [Installing ESP32 in Arduino IDE](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)

### Step 4 — Flash the ESP32

Open `arduino/ESP32_L298N_MicroROS_CmdVel_WiFi/ESP32_L298N_MicroROS_CmdVel_WiFi.ino` and edit these **4 lines**:

```cpp
const char* ssid       = "YOUR_WIFI_SSID";      // Your WiFi name
const char* password   = "YOUR_WIFI_PASSWORD";  // Your WiFi password
const char* agent_ip   = "192.168.1.100";        // Your PC's IP (run: hostname -I)
const int   agent_port = 8888;                   // Keep as 8888
```

In Arduino IDE:
- **Tools → Board** → `ESP32 Dev Module`
- **Tools → Port** → `/dev/ttyUSB0` (or `/dev/ttyACM0`)
- Click **Upload ▶**

> If port is not visible: `sudo usermod -aG dialout $USER` (then log out and back in)

### Step 5 — Run Every Session

```bash
# Terminal 1 — Micro-ROS Agent
./scripts/run_agent_wifi.sh

# Terminal 2 — Keyboard Control
./scripts/run_teleop.sh

# Power on ESP32 → auto-connects WiFi → LED blinks 1 Hz → ready! ✅
```

---

## 📐 Wiring & Pin Connections

### ESP32 → L298N

| ESP32 GPIO | L298N Pin | Wire | Function |
|-----------|-----------|------|----------|
| `GPIO 5`  | `ENA`     | 🔵 Blue   | Right Motor Speed (PWM) |
| `GPIO 18` | `IN1`     | 🟢 Green  | Right Motor Direction 1 |
| `GPIO 19` | `IN2`     | 🟡 Yellow | Right Motor Direction 2 |
| `GPIO 4`  | `ENB`     | 🔵 Blue   | Left Motor Speed (PWM) |
| `GPIO 16` | `IN3`     | 🟢 Green  | Left Motor Direction 1 |
| `GPIO 17` | `IN4`     | 🟡 Yellow | Left Motor Direction 2 |
| `GND`     | `GND`     | ⚫ Black  | Common Ground (**required!**) |

### L298N → Motors & Power

| L298N Terminal | Connection |
|---------------|-----------|
| `OUT1` & `OUT2` | Right Motor |
| `OUT3` & `OUT4` | Left Motor |
| `12V` input | Battery positive |
| `GND` | Battery negative + ESP32 GND |
| `5V` output | ESP32 5V pin *(optional)* |

> ⚠️ **Always connect common GND** between ESP32 and L298N — without it, motor direction signals will not work correctly.

> 📌 Reference: [ESP32 Pinout Reference](https://lastminuteengineers.com/esp32-pinout-reference/) | [L298N Tutorial](https://youtu.be/2JTMqURJTwg)

---

## 💻 ESP32 Code Overview

### System Data Flow

```
[ Keyboard ]
     │
     ▼
[ teleop_twist_keyboard ]
     │  /cmd_vel  (geometry_msgs/Twist)
     ▼
[ micro_ros_agent ]  ← runs on PC  (UDP port 8888)
     │
   WiFi (UDP)
     │
     ▼
[ ESP32 + Micro-ROS ]
     │
     ├──→ Left Motor  (ENB / IN3 / IN4)
     └──→ Right Motor (ENA / IN1 / IN2)
```

### Differential Drive Mixing

```cpp
// /cmd_vel provides:
//   linear.x  → forward/backward  (+positive = forward)
//   angular.z → rotation          (+positive = turn left / CCW)

float left_speed  = linear_x - angular_z;
float right_speed = linear_x + angular_z;

// Both constrained to [-1.0, +1.0]
// Multiplied by SPEED_SCALE (default 0.8)
// Converted to PWM 0–255 via ledcWrite()
```

| `linear.x` | `angular.z` | Robot Motion |
|------------|-------------|--------------|
| `+` | `0` | Forward |
| `-` | `0` | Backward |
| `0` | `+` | Spin Left (CCW) |
| `0` | `-` | Spin Right (CW) |
| `+` | `+` | Forward + Left arc |
| `+` | `-` | Forward + Right arc |
| `0` | `0` | Stop |

### Connection State Machine (WiFi Sketch)

```
  ┌─────────────────┐
  │  WAITING_AGENT  │ ← LED slow blink, ping agent every 1s
  └────────┬────────┘
    ping OK │
            ▼
  ┌─────────────────┐
  │ AGENT_AVAILABLE │ ← Create node, subscriber, timer, executor
  └────────┬────────┘
   success │
            ▼
  ┌─────────────────┐
  │ AGENT_CONNECTED │ ← LED 1Hz blink, spin executor, drive motors
  └────────┬────────┘
  ping fail│
            ▼
  ┌─────────────────────┐
  │ AGENT_DISCONNECTED  │ ← Stop motors, destroy entities
  └──────────────────┬──┘
                     └──────────────────────→ WAITING_AGENT
```

### Key Configuration

```cpp
// ── WiFi ────────────────────────────────────────────
const char* ssid       = "YOUR_WIFI_SSID";
const char* password   = "YOUR_WIFI_PASSWORD";
const char* agent_ip   = "192.168.1.100";   // PC IP
const int   agent_port = 8888;

// ── Motor Pins ──────────────────────────────────────
#define ENA  5    // Right Motor PWM speed
#define IN1  18   // Right Motor Direction 1
#define IN2  19   // Right Motor Direction 2
#define ENB  4    // Left Motor PWM speed
#define IN3  16   // Left Motor Direction 1
#define IN4  17   // Left Motor Direction 2

// ── Tuning ──────────────────────────────────────────
#define SPEED_SCALE  0.8   // Reduce if robot moves too fast (0.0–1.0)
#define PWM_FREQ     5000  // 5 kHz PWM
#define PWM_RES      8     // 8-bit → 0–255
```

---

## 🎮 Keyboard Controls (Teleop)

```
   u    i    o
   j    k    l
   m    ,    .
```

| Key | Action |
|-----|--------|
| `i` | ⬆️ Forward |
| `,` | ⬇️ Backward |
| `j` | ⬅️ Turn Left |
| `l` | ➡️ Turn Right |
| `k` | ⏹️ **STOP** |
| `u` / `o` | ↖️ / ↗️ Forward diagonal |
| `m` / `.` | ↙️ / ↘️ Backward diagonal |
| `q` / `z` | Increase / decrease max speed |
| `w` / `x` | Increase / decrease linear speed only |
| `e` / `c` | Increase / decrease angular speed only |

---

## 💡 LED Status Reference

| LED Pattern | Meaning |
|-------------|---------|
| ⚡ Fast blink × 5 | Startup — GPIO initializing |
| 🔵 Slow blink | Connecting to WiFi... |
| 🟢 Steady ON | WiFi connected, waiting for agent |
| 🟢 Regular blink (1 Hz) | ✅ **Ready** — connected to ROS2 agent |
| 🔴 Fast continuous blink | ❌ **ERROR** — check agent & serial monitor |

---

## 🌐 Network Configuration

```
WiFi Router:  192.168.1.1
  ├── PC (Ubuntu):  192.168.1.100   ← runs micro-ros-agent on UDP :8888
  └── ESP32:        192.168.1.150   ← auto-assigned via DHCP
```

### Find Your PC's IP

```bash
hostname -I
# or: ip addr show wlan0 | grep "inet "
```

### Firewall Rules

```bash
sudo ufw allow 8888/udp
sudo ufw status
```

### Optional: Static IP for ESP32

Add before `WiFi.begin()` in the sketch:

```cpp
IPAddress local_IP(192, 168, 1, 150);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
WiFi.config(local_IP, gateway, subnet);
```

---

## ✅ WiFi vs Serial

| Feature | WiFi (UDP) ✅ Recommended | Serial (USB) |
|---------|--------------------------|-------------|
| Cable required | ❌ No | ✅ Yes |
| Range | 50+ meters | USB length |
| Auto-reconnect | ✅ Yes (state machine) | ❌ Manual reset |
| Multiple robots | ✅ Yes | One per port |
| Latency | ~1–5 ms | ~1–2 ms |
| Best for | Final use, demos | Initial setup, debug |

---

## 🔧 Troubleshooting

<details>
<summary>❌ Garbled Serial output ( ▯▯▯▯▯▯ )</summary>

**Cause:** Baud rate mismatch — code uses `115200` but Serial Monitor is set to `9600`

**Fix:**
1. In Arduino IDE → Serial Monitor → change `9600` → **`115200`**
2. Press **RESET** on ESP32

> This is a Serial Monitor display issue only — nothing to do with WiFi or Micro-ROS.

</details>

<details>
<summary>❌ ESP32 won't connect to WiFi</summary>

- Double-check SSID and password (case-sensitive)
- Make sure your router broadcasts **2.4 GHz** — ESP32 does not support 5 GHz
- Add debug output:

```cpp
// After Serial.begin(115200):
Serial.print("WiFi status: ");
Serial.println(WiFi.status());
// 3 = connected, 6 = wrong password, 1 = no SSID found
```

</details>

<details>
<summary>❌ micro-ROS agent not receiving data</summary>

```bash
# 1. Confirm your PC's IP matches the sketch
hostname -I

# 2. Allow UDP port through firewall
sudo ufw allow 8888/udp

# 3. Run agent with maximum verbosity
ros2 run micro_ros_agent micro_ros_agent udp4 --port 8888 -v6

# 4. Verify port is open and listening
sudo netstat -tulpn | grep 8888

# 5. Both devices must be on the same network
ping <ESP32_IP_ADDRESS>
```

</details>

<details>
<summary>❌ Motors not responding</summary>

- Verify GPIO numbers in sketch match your actual wiring
- Ensure **GND** is connected between ESP32 and L298N (common ground)
- Check L298N has 12V power from battery
- Remove ENA/ENB jumpers from L298N — the code controls speed via PWM
- Try increasing `SPEED_SCALE` closer to `1.0`
- Test motors independently with a simple blink-style Arduino sketch first

</details>

<details>
<summary>❌ Connection drops frequently</summary>

Add these lines after `WiFi.begin()`:

```cpp
WiFi.setSleep(false);                    // Disable WiFi power save mode
WiFi.setTxPower(WIFI_POWER_19_5dBm);   // Maximum transmit power
```

</details>

<details>
<summary>❌ colcon build fails — FastDDS target conflict</summary>

**Error message:**
```
Some (but not all) targets in this export set were already defined.
Targets Defined:     eProsima_atomic
Targets not yet defined: fastrtps
```

**Root cause:** Ubuntu ships FastRTPS 2.5.x; ROS Humble needs FastDDS 2.6.x — both are installed simultaneously and CMake gets confused.

**Fix — run in order:**

```bash
# 1. Remove Ubuntu's conflicting FastRTPS packages
sudo apt remove --purge -y \
  libfastrtps-dev libfastrtps2.5 \
  libfastcdr-dev libfastcdr1

# 2. Clean leftover CMake config files
sudo rm -rf /usr/lib/x86_64-linux-gnu/cmake/fastrtps
sudo rm -rf /usr/lib/x86_64-linux-gnu/cmake/fastdds
sudo ldconfig

# 3. Reinstall correct ROS Humble DDS packages
sudo apt install --reinstall -y \
  ros-humble-fastrtps \
  ros-humble-fastcdr \
  ros-humble-rmw-fastrtps-cpp

# 4. Wipe workspace and rebuild from scratch
cd ~/microros_ws
rm -rf build install log

# 5. Source ONLY ROS Humble (NOT /usr/local/setup.bash)
source /opt/ros/humble/setup.bash

# 6. Rebuild
colcon build --symlink-install
```

> ⚠️ **Rule of Thumb:** Never install `libfastrtps-dev` from Ubuntu repos when using ROS 2 Humble. Always use `ros-humble-fastrtps`.

</details>

<details>
<summary>❌ /dev/ttyUSB0 not visible or permission denied</summary>

```bash
# List all serial devices
ls /dev/tty*

# Add yourself to the dialout group (one-time — requires logout/login)
sudo usermod -aG dialout $USER

# Quick temporary fix (resets on reboot)
sudo chmod 666 /dev/ttyUSB0
```

</details>

---

## 📋 Quick Reference Card

Once everything is set up, use these commands every session:

```bash
# ── Terminal 1: Start Agent ──────────────────────────────────
cd ~/microros_ws && source install/setup.bash
ros2 run micro_ros_agent micro_ros_agent udp4 --port 8888

# ── Terminal 2: Keyboard Control ─────────────────────────────
source /opt/ros/humble/setup.bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard

# ── Terminal 3: Verify Everything ────────────────────────────
ros2 node list          # → /esp32_robot_wifi
ros2 topic list         # → /cmd_vel
ros2 topic info /cmd_vel  # → Subscription count: 1

# ── Manual Test Commands ─────────────────────────────────────
# Move forward
ros2 topic pub --once /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.2}}"
# Turn left
ros2 topic pub --once /cmd_vel geometry_msgs/msg/Twist "{angular: {z: 0.5}}"
# Stop
ros2 topic pub --once /cmd_vel geometry_msgs/msg/Twist "{}"
```

---

## 📖 References

| Resource | Link |
|---------|------|
| Micro-ROS Arduino Library | https://github.com/micro-ROS/micro_ros_arduino |
| Micro-ROS Agent | https://github.com/micro-ROS/micro-ROS-Agent |
| ROS2 Humble Documentation | https://docs.ros.org/en/humble/ |
| ESP32 Pinout Reference | https://lastminuteengineers.com/esp32-pinout-reference/ |
| L298N + ESP32 Video Tutorial | https://youtu.be/2JTMqURJTwg |
| Installing ESP32 in Arduino IDE | https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/ |
| teleop_twist_keyboard | https://github.com/ros2/teleop_twist_keyboard |
| Micro-ROS Docs | https://micro.ros.org/docs/overview/ |

---


<div align="center">
Made with ❤️ for the ROS2 + ESP32 robotics community
<br/><br/>
<b>Mini Bot v1.0</b> &nbsp;|&nbsp; ROS2 Humble &nbsp;|&nbsp; ESP32 &nbsp;|&nbsp; Micro-ROS
</div>
