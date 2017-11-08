/*
 * This example prints a message via Serial every time digital pin 7 is pulled to low level (via a pushbutton)
 * The button connects digital pin 7 and GND when pressed
 * The internal pullup on digital pin 7 is enabled to ensure a high level when the pushbutton is not pressed
 */
#include <Unbounce.h>
#define typeDown 0
#define typeUp 1

int pin = 7;

void buttonDown() {
  Serial.println(F("The button has been pressed!"));
}

Unbounce unbounce(pin, typeDown, buttonDown); // Pin = Digital Pin 7, typeDown = 0 --> buttonDown will be called then Digital Pin 7 transitions from High to Low (0)

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
  unbounce.handle();
}


