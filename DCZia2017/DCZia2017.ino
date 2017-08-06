/*
  DCZia Badge Test Code 2017
  @LithoChasm - First Commit
  @rlucero - Scrolling, button reset, BLE updates
  @rlucero - Clean up for upload to GitHub

  The circuit:
   LCD RS pin to digital pin 13
   LCD Enable pin to digital pin 12
   LCD D4 pin to digital pin 11
   LCD D5 pin to digital pin 10
   LCD D6 pin to digital pin 9
   LCD D7 pin to digital pin 8
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  http://www.arduino.cc/en/Tutorial/LiquidCrystal
*/

// include the library code:
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#include <CurieBLE.h>
#include <BLEDevice.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            2

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      4

// Backlight Pin. Sadly pin 7 does not do pwm. We can bit bang it if we need to.
#define lcdBacklight  7

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
const int button1Pin = 6;
const int button2Pin = 5;
const int button3Pin = 4;
const int button4Pin = 3;

// Setup BTLE Stuff
BLEPeripheral blePeripheral;
BLEService bleService();
BLEDevice bleDevice;

// NeoPixel Stuff
int brightness = 0;    // how bright the LED is
int fadeAmount = 4;    // how many points to fade the LED by


//Button State Change Code
int button1State = 1;
int button2State = 1;
int button3State = 1;
int button4State = 1;
int lastButton1State = 1;
int lastButton2State = 1;
int lastButton3State = 1;
int lastButton4State = 1;

int delayval = 25; // delay global

int lightProgram = 1;


void setup() {
  //Configure serial debugging
  Serial.begin(9600);

  //Setup unsigned char string for manufacturer id.
  //PP is 0x5050 in hex...
  String manDataString = "PP";
  int strLength = sizeof(manDataString);
  unsigned char manData[100];
  for (int i = 0; i < strLength; i++) {
    manData[i] = manDataString[i];
  }

  //BLE Device Initialization
  //NOTE: This is required to work with other DC Badges
  bleDevice.setAppearance(0);
  bleDevice.setManufacturerData(manData , 2);
  bleDevice.setLocalName("DCZia");
  bleDevice.setDeviceName("DCZia");

  //BLE Peripheral Service Initialization
  blePeripheral.setLocalName("DCZia");
  blePeripheral.setDeviceName("DCZia");
  blePeripheral.setAdvertisedServiceUuid("66200011223344556677080944435A494121");
  blePeripheral.setAppearance(0x19DC);

  // BLE advertise the service
  bleDevice.advertise();
  bleDevice.begin();
  blePeripheral.begin();

  // This initializes the NeoPixel library.
  pixels.begin();

  //Set buttons to have a pullup for stabilazation
  pinMode(6, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);


  // Backlight Setup
  pinMode(lcdBacklight, OUTPUT);
  digitalWrite(lcdBacklight, HIGH);


  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print Initialization Message
  lcd.clear();
  lcd.print(" DC ZIA // 2017 ");
  lcd.setCursor(0, 1);
  lcd.print(" ++ DEF25CON ++ ");

}


void loop() {

  //Light Programs
  while (lightProgram == 1) {
    //ChillMode
    pixels.begin();
    digitalWrite(lcdBacklight, HIGH);
    for (int i = 0; i < NUMPIXELS; i++) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      // change the brightness for next time through the loop:
      pixels.setPixelColor(i, pixels.Color(brightness % 18, brightness % 4, brightness)); // Moderately bright green color.

      brightness = brightness + fadeAmount;

      // reverse the direction of the fading at the ends of the fade:
      if (brightness <= 0 || brightness >= 125) {
        fadeAmount = -fadeAmount;

      }

      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(30); // Delay for a period of time (in milliseconds).
    }
    readButtonState();
  }

  while (lightProgram == 2) { 
    //ChileMode
    pixels.begin();
    digitalWrite(lcdBacklight, HIGH);
    for (int i = 0; i < NUMPIXELS; i++) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      // change the brightness for next time through the loop:
      pixels.setPixelColor(i, pixels.Color(brightness % 18, brightness % 10, 0)); // Moderately bright green color.
      brightness = brightness + fadeAmount;

      // reverse the direction of the fading at the ends of the fade:
      if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
      }

      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval); // Delay for a period of time (in milliseconds).

    }
    readButtonState();
  }

  while (lightProgram == 3) { 
    //Party Mode!
    pixels.begin();
    digitalWrite(lcdBacklight, HIGH);
    pixels.setPixelColor(0, pixels.Color(66, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, 66));
    pixels.setPixelColor(2, pixels.Color(66, 0, 0));
    pixels.setPixelColor(3, pixels.Color(0, 0, 66));
    pixels.show();
    delay(80);
    pixels.setPixelColor(0, pixels.Color(0, 0, 66));
    pixels.setPixelColor(1, pixels.Color(66, 0, 0));
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.setPixelColor(3, pixels.Color(66, 0, 0));
    pixels.show();
    delay(80);
    readButtonState();
  }

  while (lightProgram == 4) {
    digitalWrite(lcdBacklight, HIGH);
    pixels.setPixelColor(0, pixels.Color(0, 0, 35));
    pixels.setPixelColor(1, pixels.Color(0, 50, 35));
    pixels.setPixelColor(2, pixels.Color(0, 0, 35));
    pixels.setPixelColor(3, pixels.Color(0, 50, 35));
    pixels.show();
    delay(40);
    readButtonState();
  }

  while (lightProgram == 5) {

    digitalWrite(lcdBacklight, HIGH);
    for (int i = 0; i < NUMPIXELS; i++) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      // change the brightness for next time through the loop:
      pixels.setPixelColor(i, pixels.Color(brightness % 3, 0, brightness % 4)); // Moderately bright green color.
      brightness = brightness + fadeAmount;

      // reverse the direction of the fading at the ends of the fade:
      if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
      }

      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval); // Delay for a period of time (in milliseconds).

    }
    delay(40);
    readButtonState();

  }


  while (lightProgram == 6) {
    digitalWrite(lcdBacklight, LOW);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.setPixelColor(3, pixels.Color(0, 0, 0));
    pixels.show();
    delay(40);
    readButtonState();
  }
}


void readButtonState() {
  // Read button states
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button3State = digitalRead(button3Pin);
  button4State = digitalRead(button4Pin);

  if (button1State == LOW && button4State == LOW) {
    lcd.clear();
    lcd.begin(16, 2);
    lcd.print("Night Mode On");
    lcd.setCursor(0, 1);
    lcd.print("   (>_>) Z  ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("   (<_<) ZZ  ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("   (>_>) ZZZ  ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("   (<_<) ZZZZ  ");
    delay(3000);
    lcd.clear();
    lightProgram = 6;
    return;
  }

  if (button2State == LOW && button3State == LOW) {
    lcd.clear();
    lcd.begin(16, 2);
    lcd.print(" DC ZIA // 2017 ");
    lcd.setCursor(0, 1);
    lcd.print("     >-.-<     ");

    lightProgram = 5;
    return;
  }

  if (button1State != lastButton1State) { 
    // If button state has changed
    Serial.print(" BUTTON 1 STATE CHANGE");
    if (button1State == LOW) {
      lcd.begin(16, 2);
      // Print Initialization Message
      lcd.clear();
      brightness = 0;
      fadeAmount = 5;
      lcd.print(" DC ZIA // 2017 ");
      lcd.setCursor(0, 1);
      lcd.print("  >Chill Mode<  ");
      Serial.print("BUTTON 1!!!!");
      lightProgram = 1;
    }
    delay(10);
  }
  lastButton1State = button1State;

  if (button2State != lastButton2State) { 
    if (button2State == LOW) {
      brightness = 0;
      fadeAmount = 5;
      lcd.begin(16, 2);
      // Print Initialization Message
      lcd.clear();
      lcd.print(" DC ZIA // 2017 ");
      lcd.setCursor(0, 1);
      lcd.print("<><><><><><><><>");

      lightProgram = 2;
    }
    delay(10);
    lastButton2State = button2State;
  }

  if (button3State != lastButton3State) { // If button state has changed
    if (button3State == LOW) {
      lcd.begin(16, 2);
      // Print Initialization Message
      lcd.clear();
      lcd.print(" DC ZIA // 2017 ");
      lcd.setCursor(0, 1);
      lcd.print("  !Party Mode! ");
      lightProgram = 3;
    }
    delay(10);
  }
  lastButton3State = button3State;

  if (button4State != lastButton4State) { // If button state has changed
    if (button4State == LOW) {
      lcd.begin(16, 2);
      // Print Initialization Message
      lcd.clear();
      lcd.print(" DC ZIA // 2017 ");
      lcd.setCursor(0, 1);
      lcd.print(" (^_^)");
      lightProgram = 4;
    }
    delay(10);
  }
  lastButton4State = button4State;
}



