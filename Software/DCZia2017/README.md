# DC-Zia 2017 Badge
This is the sketch code for the DC Zia 2017 badge. It requires sketch code, an Arduino 101 and updated BLE Driver code.

## Requirements
* [Arduino 101](https://www.amazon.com/Intel-Arduino-Development-Board-Curie/dp/B0198HHR06)
* DC Zia 2017 Shield
* Sketch Code
* Updated BLE Drivers (Only required for interacting with other DC2017 badges)

## Instructions
* Assemble the shield
* Open the sketch
* From the Tools Menu select: Board Manager - Arduino/Genduino 101 (NOTE: please update to the 2.0.2 version via Board Manager
* Update `%LOCALAPPDATA%\Arduino15\packages\Intel\hardware\arc32\2.0.2\libraries\CurieBLE\src\internal` with the two files included in this repo 
(NOTE: Only required for interacting with other DC 2017 Badges and it will HARDCODE your GAP Appearance ID to 0xDC19.)
* Deploy code and enjoy!