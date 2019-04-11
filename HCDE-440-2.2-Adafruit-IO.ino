// The code below send analog values to Adafruit IO froma photocell and digital values from a button.

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

// digital pin 5
#define BUTTON_PIN 5
// analog pin 0
#define PHOTOCELL_PIN A0

// button state
bool buttonCurrent = false;
bool buttonLast = false;

// photocell state
int photoCurrent = 0;
int photoLast = -1; 

// set up the 'digital' feed (button)
AdafruitIO_Feed *digital = io.feed("button");

// set up the 'analog' feed (photocell)
AdafruitIO_Feed *analog = io.feed("analog");

void setup() {

  // set button pin as an input
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // start the serial connection
  Serial.begin(115200);
  Serial.print("This board is running: ");
  Serial.println(F(__FILE__));
  Serial.print("Compiled: ");
  Serial.println(F(__DATE__ " " __TIME__));
   
  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // grab the buttonCurrent state of the photocell
  photoCurrent = analogRead(PHOTOCELL_PIN);

  // return if the value hasn't changed
  if (photoCurrent == photoLast)
    return;

  // Save the buttonCurrent state to the analog feed
  // Uncomment the code below (74 - 76) to interact with the photocell 
  // Comment the code below if you want to play with the button
  // Serial.print("sending light -> ");
  // Serial.println(photoCurrent);
  // analog->save(photoCurrent);

  // store buttonLast photocell state
  photoLast = photoCurrent;

  // grab the buttonCurrent state of the button.
  // we have to flip the logic because we are
  // using a pullup resistor.
  if(digitalRead(BUTTON_PIN) == LOW)
    buttonCurrent = true;
  else
    buttonCurrent = false;

  // return if the value hasn't changed
  if(buttonCurrent == buttonLast)
    return;

  // save the buttonCurrent state to the 'digital' feed on adafruit io
  // Uncomment the code below (96 - 98) to interact with the button
  // Comment the code below if you want to play with the photocell
  Serial.print("sending button -> ");
  Serial.println(buttonCurrent);
  digital->save(buttonCurrent);

  // store buttonLast button state
  buttonLast = buttonCurrent;

}
