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
* Intel's BLE library does not actually set the Appearance in the advertisment packet.  You need this set to 0xDC19 in the advertisement so that other defcon 25 badges will recognize it.  To do this edit `%LOCALAPPDATA%\Arduino15\packages\Intel\hardware\arc32\2.0.2\libraries\CurieBLE\src\internal\BLEDeviceManager.cpp` and add the + lines starting at line 417:

    _adv_type = (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR);
    ret = setAdvertiseData (BT_DATA_FLAGS, &_adv_type, sizeof(_adv_type));
    
+    if(_appearance != 0 && 
+        (BLE_STATUS_SUCCESS == ret)){
+        ret = setAdvertiseData(BT_DATA_GAP_APPEARANCE, (const uint8_t *)&_appearance, sizeof(_appearance));
+    }

    if (_has_service_solicit_uuid && 
        (BLE_STATUS_SUCCESS == ret)) 
    {


* Deploy code and enjoy!