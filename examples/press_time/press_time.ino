/*
 * This example measures the time digital pin 7 is pulled to low level (via a pushbutton)
 * The button connects digital pin 7 and GND when pressed
 * The internal pullup on digital pin 7 is enabled to ensure a high level when the pushbutton is not pressed
 */
#include <Unbounce.h>
#define typeDown 0
#define typeUp 1

int pin = 7;
unsigned long timestamp;

void buttonDown() {
  timestamp = millis(); // remember when the button has been pressed
  Serial.println("The button has been pressed!");
}

void buttonUp() {
  Serial.print("The button has been released after ");
  Serial.print(millis() - timestamp); // calculate the time difference between now and the button press
  Serial.println("ms");
}

Unbounce unbounceDown(pin, typeDown, buttonDown); // Pin = Digital Pin 7, typeDown = 0 --> buttonDown will be called then Digital Pin 7 transitions from High to Low (0)
Unbounce unbounceUp(pin, typeUp, buttonUp); // Pin = Digital Pin 7, typeUp = 1 --> but will be called then Digital Pin 7 transitions from Low to High (1)

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  pinMode(7, INPUT_PULLUP);
}

void loop()
{
  unbounceDown.handle();
  unbounceUp.handle();
}


