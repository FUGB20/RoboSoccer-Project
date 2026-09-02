# ESP32 RoboSoccer Robot

An ESP32-based mobile robot designed for a RoboSoccer competition,
controlled wirelessly using a PlayStation 4 controller over Bluetooth.

## 🥈 Competition Result

**2nd Place --- RoboSoccer Competition**

This project was developed as the capstone project of an Arduino \&
Robotics course, combining CAD and 3D printing, electronics, embedded
C++, and robotics into one working system.

## 🤖 Project Overview

The robot uses an **ESP32 NodeMCU** as its main microcontroller. A
**PlayStation 4 controller** connects wirelessly to the ESP32 through
Bluetooth using the **Bluepad32** library.

The robot uses a **differential-drive system** with two independently
controlled rear DC gearmotors. The controller's left analog stick is
used for throttle and steering, which are mixed into individual left and
right motor speeds.

The chassis was designed as a **double-deck 3D-printed structure** with
the battery positioned low in the chassis to improve stability. A curved
front bumper helps retain and control the ball.

## 🛠️ Main Components

\---

Component                           Role

\---

ESP32 NodeMCU (30-pin)              Main microcontroller and Bluetooth
communication

PlayStation 4 DualShock 4           Wireless robot controller

Bluepad32                           Gamepad communication library

L298N H-Bridge                      DC motor driver

2 × Yellow DC Gearmotors (1:48)     Rear-wheel drive

3 × 18650 Li-ion cells              Power source

3D-printed chassis                  Mechanical structure

## Metal ball caster                   Front passive support

## ⚙️ System Architecture

``` text
PlayStation 4 Controller
          │
       Bluetooth
          │
          ▼
   ┌──────────────┐
   │     ESP32    │
   │  Bluepad32   │
   └──────┬───────┘
          │
    PWM + Direction
          │
          ▼
   ┌──────────────┐
   │    L298N     │
   │  H-Bridge    │
   └──────┬───────┘
          │
     ┌────┴────┐
     ▼         ▼
 Left Motor  Right Motor
```

## 🎮 Control System

The robot uses an **Arcade Drive** control scheme.

* Left joystick **Y-axis** → throttle
* Left joystick **X-axis** → steering
* A joystick deadzone of **±30** filters small unwanted movements.
* Motor speeds are calculated using:

``` text
Left Motor  = Throttle + Steering
Right Motor = Throttle - Steering
```

The resulting values are constrained to the joystick's operating range
and converted into PWM values for motor speed control.

## 🔌 ESP32 ↔ L298N Pinout

&#x20;   ESP32 GPIO L298N Pin   Function

\---

&#x20;      GPIO 14 IN1         Left motor direction
GPIO 12 IN2         Left motor direction
GPIO 25 ENA         Left motor PWM
GPIO 32 IN3         Right motor direction
GPIO 33 IN4         Right motor direction
GPIO 26 ENB         Right motor PWM



The ESP32, L298N, and battery share a **common ground**.

## 🔋 Power System

The robot uses **three 18650 Li-ion cells in series**, providing a
nominal **11.1 V** supply.

The battery powers the L298N motor driver, while the system uses the
L298N board's 5 V output to supply the ESP32.

> \\\*\\\*Important:\\\*\\\* Li-ion batteries require appropriate charging,
> protection, and handling. Verify your specific battery holder, cells,
> motor driver, and power wiring before operating the robot.

## 🧩 Mechanical Design

The chassis was designed in **SOLIDWORKS** and fabricated using **3D
printing**.

Key design features include:

* Double-deck chassis
* Four rectangular structural standoffs
* Low battery placement for a lower center of gravity
* Two rear drive motors
* Front metal ball caster
* Curved double-tier front bumper for ball control

## 💻 Software

The firmware is written in **C++** using the **Arduino IDE** ecosystem.

### Main software responsibilities

* Initialize the ESP32 and motor-control pins
* Establish Bluetooth gamepad communication
* Detect controller connection and disconnection
* Read joystick inputs
* Apply joystick deadzones
* Mix throttle and steering
* Generate motor direction signals
* Generate PWM speed signals
* Stop the motors when the controller disconnects

### Libraries

* [Bluepad32](https://github.com/ricardoquesada/bluepad32)

