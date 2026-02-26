# Differential-Drive-RC-Robot-Controlled-via-ROS2-Humble-ROS2-ESP32-
Design and develop a differential-drive robot that can be controlled wirelessly using ROS2 Humble through the teleop_twist_keyboard package.

# Complete Guide: ESP32 Robot Control with ROS2 Humble + Micro-ROS by Milind Late

I'll guide you through setting up ESP32 with Micro-ROS to control your bot using cmd_vel and teleop keyboard.

Prerequisites:

    • ROS 2 Humble installed 
    • ESP32 board 
    • Arduino IDE installed 
    • Ubuntu 22.04 (recommended for Humble) 

Step 1: Setup ROS2 Workspace
bash
# Create workspace:
mkdir -p ~/microros_ws/src
cd ~/microros_ws/src

# Source ROS2:
source /opt/ros/humble/setup.bash

Step 2: Install Micro-ROS Agent
bash
# Install dependencies:
sudo apt update
sudo apt install python3-pip

# Navigate to workspace:
cd ~/microros_ws

# Clone micro-ROS agent {change according to your ros type}
git clone -b humble https://github.com/micro-ROS/micro-ROS-Agent.git src/micro-ROS-Agent

# Install rosdep if not installed:
sudo apt install python3-rosdep2
sudo rosdep init  # Skip if already initialized
rosdep update

# Install dependencies
rosdep install --from-paths src --ignore-src -y

# Build the workspace
colcon build
source install/setup.bash
```

---

# Step 3: Setup ESP32 in Arduino IDE

3.1: Install ESP32 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. Add this URL to "Additional Board Manager URLs":
```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    4. Go to Tools → Board → Boards Manager 
    5. Search for "ESP32" and install "ESP32 by Espressif Systems"{choose ESP32 Dev & Version 2.0.2 }








https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/ ---→ {refrence}

3.2: Install Micro-ROS Arduino Library
    1. Go to Sketch → Include Library → Manage Libraries 
    2. Search for "micro_ros_arduino" 
    3. Install the latest version 
Alternative Method (if library manager fails):
bash
cd ~/Arduino/libraries
git clone https://github.com/micro-ROS/micro_ros_arduino.git
