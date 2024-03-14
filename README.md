# Directional Joystick On/Off switches!
This tutorial will guide you on how to set up your directional Joystick switch!

## Things used in this project

### Hardware components
x1 ESP32-C3-Devkit (must be able to connect to WiFi)
x1 Analog potentiometer Joystick
x3 LED Diodes ("Appliances to On/Off")

### Software apps and online services
Arduino IDE (with Firebase library installed)

## Story
Wouldn't it be cool if we could turn appliances on in a 'StarCraft deploy' fasion?!

The analog joystick here is the main device. Whenever the user bumps the joystick in a certain direction and releases, it triggers to switch on the 'appliance' (simulated by the LED diodes) in the bumped direction.

This is achieved by:
1) Updating the status of the appliance on the database everytime the joystick is bumped
2) Reading from the database in fixed intervals, turning on/off based on the status read

There are many ways to turn on appliances, but rarely does it compare to the satisfaction of flicking a joystick in a chosen direction, to quite literally send out our commands (to On/Off) 😎

### To do it yourself too
The Arduino code has already been uploaded! 
Before going into the code, you first need to download this library: https://github.com/mobizt/Firebase-ESP32
### Directional Joystick On/Off switches!
 
### ESP32-C3 Devkit Pinouts
![image](https://github.com/ChenYirongSUTD/IOT_Yirong_JoystickSwitch/assets/130292439/cf954921-a9ab-405a-8efd-f4777e6f6ae4)

### Pin Connections
Joystick VRY - A0

Joystick VRX - A1

Joystick SW - GPIO 3
Yellow LED - GPIO 2
Green LED - GPIO 9
Red LED - GPIO 7

To get started with setting up your Firebase Realtime Database and other basic ESP functions to set and get data values from your database:
https://randomnerdtutorials.com/esp32-firebase-realtime-database/

Have a good time playing with the code!
* The joystick used in this example had a normal resting xvalue & yvalue of ~3400
* xvalue < 1000, xvalue > 3600 and yvalue <1000 was used as threshold to detect the "bump" in a direction
